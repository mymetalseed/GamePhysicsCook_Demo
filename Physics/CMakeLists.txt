# CMakeList.txt: Physics 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("Physics")

# 将源代码添加到此项目的可执行文件。
add_executable (Physics "Physics.cpp" "Physics.h" "Math/vectors.h" "Math/vectors.cpp" "Math/matrices.cpp" "Math/matrices.h" "Math/Compare.h" "Application/Geometry2D.cpp" "Application/Geometry2D.h" "Application/QuadTree.h" "Application/QuadTree.cpp" "Application/Geometry3D.h" "Application/Geometry3D.cpp" "Application/Scene.h" "Application/Scene.cpp" "Application/Camera.h" "Application/Camera.cpp")

# TODO: 如有需要，请添加测试并安装目标。
