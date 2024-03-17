#ifndef _H_PHYSICS_SYSTEM_
#define _H_PHYSICS_SYSTEM_

#include "Rigidbody.h"
#include "Spring.h"
#include "Cloth.h"

class PhysicsSystem
{
protected:
	std::vector<Rigidbody*> bodies;
	std::vector<OBB> constraints;
	std::vector<Rigidbody*> colliders1;
	std::vector<Rigidbody*> colliders2;
	std::vector<CollisionManifold> results;
	std::vector<Spring> springs;
	std::vector<Cloth*> cloths;
	
	float LinearProjectionPercent;
	float PenetrationSlack;
	//[1 to 20]
	int ImpulseIteration;

public:
	inline PhysicsSystem();
	void Update(float deltaTime);
	void Render();

	void AddRigidbody(Rigidbody* body);
	void AddConstraint(const OBB& constraint);

	void ClearRigidbodys();
	void ClearConstraints();
	void AddSpring(const Spring& spring);
	void ClearSprings();

	//Cloth
	void AddCloth(Cloth* cloth);
	void ClearCloths();
};

#endif // !_H_PHYSICS_SYSTEM_
