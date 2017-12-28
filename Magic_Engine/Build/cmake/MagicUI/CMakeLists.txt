include_directories(
../../../Plugins/glm
../../../Core/Magic_Engine/Include
../../../Core/MagicUI/Include
../../../Core/MagicUI)

link_directories(
${PROJECT_SOURCE_DIR}../../../lib/${CMAKE_SYSTEM_NAME}/)

link_libraries(Magic_Engine.lib)

set(INCLUDE_FILES
${PROJECT_SOURCE_DIR}../../../Core/MagicUI/Include/MagicUIAPI.h
${PROJECT_SOURCE_DIR}../../../Core/MagicUI/Include/MagicUIMacro.h
)

set(SOURCE_FILES
${PROJECT_SOURCE_DIR}../../../Core/MagicUI/DebugScene.cpp
${PROJECT_SOURCE_DIR}../../../Core/MagicUI/DebugScene.h
${PROJECT_SOURCE_DIR}../../../Core/MagicUI/MagicUIAPI.cpp
${PROJECT_SOURCE_DIR}../../../Core/MagicUI/MagicWindows.cpp
${PROJECT_SOURCE_DIR}../../../Core/MagicUI/MagicWindows.h
${PROJECT_SOURCE_DIR}../../../Core/MagicUI/MenuScene.cpp
${PROJECT_SOURCE_DIR}../../../Core/MagicUI/MenuScene.h
${PROJECT_SOURCE_DIR}../../../Core/MagicUI/SystemScene.cpp
${PROJECT_SOURCE_DIR}../../../Core/MagicUI/SystemScene.h
${PROJECT_SOURCE_DIR}../../../Core/MagicUI/UserScene.cpp
${PROJECT_SOURCE_DIR}../../../Core/MagicUI/UserScene.h)

SOURCE_GROUP("Include Files" FILES ${INCLUDE_FILES})
SOURCE_GROUP("Source Files" FILES ${SOURCE_FILES})

add_definitions(-DMAGICUI_EXPORTS)
IF(WIN32)
	add_definitions(-DUNICODE -D_UNICODE)
ENDIF(WIN32)

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(../../../Core/MagicUI DIR_LIB_SRCS)

set(NOW_PROJECT_NAME MagicUI)
# 生成链接库
#add_library (MagicUI STATIC ${DIR_LIB_SRCS})
add_library (${NOW_PROJECT_NAME} SHARED ${DIR_LIB_SRCS} ${INCLUDE_FILES} ${SOURCE_FILES})

#RUNTIME	.exe、.dll	可执行档
SET_TARGET_PROPERTIES(${NOW_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../../../bin/${CMAKE_SYSTEM_NAME})
#LIBRARY		.so
#SET_TARGET_PROPERTIES(${NOW_PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../../../lib)
#ARCHIVE	.a、.lib包含 dll 的连结介面	.a
SET_TARGET_PROPERTIES(${NOW_PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ../../../lib/${CMAKE_SYSTEM_NAME})

#添加顶级目标之间的依赖关系。
add_dependencies(${NOW_PROJECT_NAME} Magic_Engine)