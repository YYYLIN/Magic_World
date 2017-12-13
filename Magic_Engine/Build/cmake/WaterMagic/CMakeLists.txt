include_directories(
../../../Plugins/glm
../../../Core/Magic_Engine/Include
../../../Core/MagicUI/Include
../../../Core/WaterMagic
../../../Plugins/Structure)

link_directories(
${PROJECT_SOURCE_DIR}../../../lib/Win32/)

set(SCENE_FILES
${PROJECT_SOURCE_DIR}../../../Core/WaterMagic/MainScene.cpp
${PROJECT_SOURCE_DIR}../../../Core/WaterMagic/MainScene.h)

SOURCE_GROUP("Scene" FILES ${SCENE_FILES})

link_libraries(Magic_Engine.lib MagicUI.lib)

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(../../../Core/WaterMagic DIR_LIB_SRCS)

ADD_EXECUTABLE (WaterMagic ${DIR_LIB_SRCS} ${SCENE_FILES})

#RUNTIME	.exe、.dll	可执行档
SET_TARGET_PROPERTIES(WaterMagic PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../../../bin)
#LIBRARY		.so
#SET_TARGET_PROPERTIES(WaterMagic PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../../../lib)
#ARCHIVE	.a、.lib包含 dll 的连结介面	.a
SET_TARGET_PROPERTIES(WaterMagic PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ../../../lib)