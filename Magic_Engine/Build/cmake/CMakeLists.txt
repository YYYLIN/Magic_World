# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
project (Magic_World)
# 加入一个配置头文件，用于处理 CMake 对源码的设置
#configure_file (
#  "${PROJECT_SOURCE_DIR}/config.h.in"
#  "${PROJECT_BINARY_DIR}/config.h"
#  )
# 配置时是否定义这个宏
#option (USE_MYMATH "Use provided math implementation" ON)
	   
# 是否加入 Magic_Engine 库
#if (USE_MYMATH)
#  include_directories ("${PROJECT_SOURCE_DIR}/math")
#  add_subdirectory (math)  
#  set (EXTRA_LIBS ${EXTRA_LIBS} Magic_Engine)
#endif (USE_MYMATH)	   

# 添加 Magic_Engine 子目录
add_subdirectory(Magic_Engine)
add_subdirectory(MagicGrating)
add_subdirectory(MagicUI)
add_subdirectory(WaterMagic)

set(CMAKE_BUILD_TYPE "Debug")
#set (CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX} "C:/Program Files (x86)/")
#set (EXTRA_LIBS ${EXTRA_LIBS} Magic_Engine)

MESSAGE(STATUS "This is BINARY dir " ${PROJECT_BINARY_DIR})
MESSAGE(STATUS "This is SOURCE dir " ${PROJECT_SOURCE_DIR})

# 添加链接库
#target_link_libraries(Demo Magic_Engine)
