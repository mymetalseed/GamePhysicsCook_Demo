#include "QuadTree.h"
#include <queue>

int QuadTreeNode::maxDepth = 5;
int QuadTreeNode::maxObjectsPerNode = 15;

bool QuadTreeNode::IsLeaf() {
	return children.size() == 0;
}

int QuadTreeNode::NumObjects() {
	Reset();

	int objectCount = contents.size();
	for (int i = 0, size = contents.size(); i < size; ++i) {
		contents[i]->flag = true;
	}
	//bfs遍历当前所有的节点
	std::queue<QuadTreeNode*> process;
	process.push(this);

	while (process.size() > 0) {
		QuadTreeNode* processing = process.back();

		if (!processing->IsLeaf()) {
			for (int i = 0, size = processing->contents.size(); i < size; ++i) {
				process.push(&processing->children[i]);
			}
		}
		else {
			for (int i = 0, size = processing->contents.size(); i < size; ++i) {
				if (!processing->contents[i]->flag) {
					objectCount += 1;
					processing->contents[i]->flag = true;
				}
			}
		}

		process.pop();
	}

	return objectCount;
}
//向当前节点插入一个节点数据
void QuadTreeNode::Insert(QuadTreeData& data) {
	//如果data的bounds和当前节点的bounds不相交，则无法插入
	if (!RectangleRectangle(data.bounds, nodeBounds)) {
		return;//The object does not fit into this node
	}
	//如果是叶子节点且已经超过了每个节点最大的对象数量,拆分
	if (IsLeaf() && contents.size() + 1 > maxObjectsPerNode) {
		Split(); // Try splitting!
	}
	//通过 是叶子节点，插入
	if (IsLeaf()) {
		contents.push_back(&data);
	}
	else {
		//不是叶子节点，插入到子节点中的每一个(?
		for (int i = 0, size = children.size(); i < size; ++i) {
			children[i].Insert(data);
		}
	}
}

void QuadTreeNode::Remove(QuadTreeData& data) {
	if (IsLeaf()) {
		int removeIndex = -1;
		for (int i = 0, size = contents.size(); i < size; ++i) {
			if (contents[i]->object == data.object) {
				removeIndex = i;
				break;
			}
		}

		if (removeIndex != -1) {
			contents.erase(contents.begin() + 1);
		}
	}
	else {
		for (int i = 0, size = children.size(); i < size; ++i) {
			children[i].Remove(data);
		}
	}

	Shake();
}

void QuadTreeNode::Update(QuadTreeData& data) {
	Remove(data);
	Insert(data);
}

//重构树结构
void QuadTreeNode::Shake() {
	//Cant shake a leaf
	if (!IsLeaf()) {
		int numObjects = NumObjects();
		if (numObjects == 0) {
			//None of the children contain any objects,collapse
			children.clear();
		}
		else if (numObjects < maxObjectsPerNode) {
			//Children combined have less objects than maximum for the node, collapse!
			std::queue<QuadTreeNode*> process;
			process.push(this);

			while (process.size() > 0) {
				QuadTreeNode* processing = process.back();

				if (!processing->IsLeaf()) {
					//Not a leaf node,add children to list to process!
					for (int i = 0, size = processing->children.size(); i < size; ++i) {
						process.push(&processing->children[i]);
					}
				}
				else {
					//Was a leaf node,add all children to our list!
					contents.insert(contents.end(), processing->contents.begin(), processing->contents.end());
				}

				process.pop();
			}

			children.clear();
		}
	}
}

void QuadTreeNode::Reset() {
	if (IsLeaf()) {
		for (int i = 0, size = contents.size(); i < size; ++i) {
			contents[i]->flag = false;
		}
	}
	else {
		for (int i = 0, size = children.size(); i < size; ++i) {
			children[i].Reset();
		}
	}
}

void QuadTreeNode::Split() {
	if (currentDepth + 1 >= maxDepth) {
		return;
	}

	vec2 min = GetMin(nodeBounds);
	vec2 max = GetMax(nodeBounds);
	vec2 center = min + ((max - min) * 0.5f);

	Rectangle2D childAreas[] = {
		Rectangle2D(FromMinMax(vec2(min.x,min.y),vec2(center.x,center.y))),
		Rectangle2D(FromMinMax(vec2(center.x,min.y),vec2(max.x,center.y))),
		Rectangle2D(FromMinMax(vec2(center.x,center.y),vec2(max.x,max.y))),
		Rectangle2D(FromMinMax(vec2(min.x,center.y),vec2(center.x,max.y))),
	};

	for (int i = 0; i < 4; ++i) {
		children.push_back(QuadTreeNode(childAreas[i]));
		children[i].currentDepth = currentDepth + 1;
	}

	for (int i = 0, size = contents.size(); i < size; ++i) {
		children[i].Insert(*contents[i]);
	}

	contents.clear();
}

std::vector<QuadTreeData*> QuadTreeNode::Query(const Rectangle2D& area) {
	std::vector<QuadTreeData*> result;
	if (!RectangleRectangle(area, nodeBounds)) {
		return result;
	}

	if (IsLeaf()) {
		for (int i = 0, size = contents.size(); i < size; ++i) {
			if(RectangleRectangle(contents[i]->bounds, area)){
				result.push_back(contents[i]);
			}
		}
	}
	else {
		for (int i = 0, size = children.size(); i < size; ++i) {
			std::vector<QuadTreeData*> recurse = children[i].Query(area);
			if (recurse.size() > 0) {
				result.insert(result.end(), recurse.begin(), recurse.end());
			}
		}
	}

	return result;
}





