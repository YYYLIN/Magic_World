include_directories(
../../../Plugins/Structure
../../../Plugins/OpenGL/H-files
../../../Plugins/lpng1621/Include
../../../Plugins/glm
../../../Plugins/glew-1.9.0/include
../../../Plugins/freetype-2.6/include
../../../Core/Magic_Engine/Include
../../../Core/Magic_Engine)

link_directories(
${PROJECT_SOURCE_DIR}../../../Plugins/OpenGL/lib/
${PROJECT_SOURCE_DIR}../../../Plugins/lpng1621/lib/
${PROJECT_SOURCE_DIR}../../../Plugins/glew-1.9.0/lib
${PROJECT_SOURCE_DIR}../../../Plugins/freetype-2.6/lib
)

set(MAGIC_ENGINE_FILES
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Include/Magic_Macro.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Include/Magic_Fonts.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Include/MagicAlphaWindow.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Include/MagicCamera.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Include/MagicEngineAPI.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Include/MagicEngineContext.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Include/MagicMath.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Include/MagicMessage.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Include/MagicModelObject.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Include/MagicPen.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Include/MagicShader.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Include/MagicTexture.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Include/MagicVertex.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Include/MagicWorld.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Magic_Fonts.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/MagicAlphaWindow.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/MagicCamera.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/MagicEngineAPI.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/MagicEngineContext.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/MagicMath.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/MagicMessage.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/MagicModelObject.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/MagicPen.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/MagicShader.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/MagicTexture.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/MagicVertex.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/MagicWorld.cpp)

SOURCE_GROUP("Engine" FILES ${MAGIC_ENGINE_FILES})

#link_libraries(Magic_Engine.lib)
add_definitions(-DMAGIC_ENGINE_EXPORTS)
IF(WIN32)
	add_definitions(-DUNICODE -D_UNICODE)
ENDIF(WIN32)


# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(../../../Core/Magic_Engine DIR_LIB_SRCS)

set(NOW_PROJECT_NAME Magic_Engine)
# 生成链接库
#add_library (Magic_Engine STATIC ${DIR_LIB_SRCS})
add_library (${NOW_PROJECT_NAME} SHARED ${DIR_LIB_SRCS} ${MAGIC_ENGINE_FILES})

#RUNTIME	.exe、.dll	可执行档
SET_TARGET_PROPERTIES(${NOW_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../../../bin/${CMAKE_SYSTEM_NAME})
#LIBRARY		.so
#SET_TARGET_PROPERTIES(${NOW_PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../../../lib)
#ARCHIVE	.a、.lib包含 dll 的连结介面	.a
SET_TARGET_PROPERTIES(${NOW_PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ../../../lib/${CMAKE_SYSTEM_NAME})