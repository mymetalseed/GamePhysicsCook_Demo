# CMakeList.txt: Physics 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("Physics")

# 我们使用Unicode字符编码
add_definitions(-D "UNICODE")
add_definitions(-D "_UNICODE")

# 查找OpenGL库
# find_package(OpenGL REQUIRED)
# include_directories(${OPENGL_INCLUDE_DIRS})
# link_libraries(${OPENGL_LIBRARIES})

# 获取程序的源文件和头文件

# add_library(glad glad/glad.c)

#imgui
#add_library(imgui imgui/imgui.cpp
#				imgui/imgui_draw.cpp
#				imgui/imgui_implementation.cpp
#				imgui/ImGuizmo.cpp
#)

#include_directories(Windows)
#include_directories(glad)
#include_directories(imgui)
#include_directories(Application)
#include_directories(Math)
#include_directories(Demos)

file(GLOB_RECURSE Windows Windows/*.cxx Windows/*.c Windows/*.cc Windows/*.cpp Windows/*.h)
file(GLOB_RECURSE Math Math/*.cxx Math/*.c Math/*.cc Math/*.cpp Math/*.h)
file(GLOB_RECURSE Application Application/*.cxx Application/*.c Application/*.cc Application/*.cpp Application/*.h)
file(GLOB_RECURSE imgui imgui/*.cxx imgui/*.c imgui/*.cc imgui/*.cpp imgui/*.h)
file(GLOB_RECURSE glad GLAD/*.cxx GLAD/*.c GLAD/*.cc GLAD/*.cpp GLAD/*.h)
file(GLOB_RECURSE Demos Demos/*.cxx Demos/*.c Demos/*.cc Demos/*.cpp Demos/*.h)

# 将目标链接到windows的一些API上
set(PLATFORM_FRAMEWORKS psapi user32 advapi32 iphlpapi userenv ws2_32)

# 将源代码添加到此项目的可执行文件。
add_executable (Physics "main-win32.cpp" ${Windows} ${Math} ${Application} ${imgui} ${glad} ${Demos})


# target_link_libraries(Physics ${OPENGL_LIBRARIES})
# target_link_libraries(Physics glad imgui)
# TODO: 如有需要，请添加测试并安装目标。
