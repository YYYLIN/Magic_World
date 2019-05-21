include_directories(
../../../Plugins/Structure
../../../Plugins/OpenGL/H-files
../../../Plugins/lpng1621/Include
../../../Plugins/glm
../../../Plugins/glew-1.9.0/include
../../../Plugins/freetype-2.6/include
../../../Plugins/EntityX
../../../Core/Magic_Engine
../../../Core/MagicThread)

link_directories(
${PROJECT_SOURCE_DIR}../../../lib/${CMAKE_SYSTEM_NAME}/
${PROJECT_SOURCE_DIR}../../../Plugins/OpenGL/lib/
${PROJECT_SOURCE_DIR}../../../Plugins/lpng1621/lib/
${PROJECT_SOURCE_DIR}../../../Plugins/glew-1.9.0/lib
${PROJECT_SOURCE_DIR}../../../Plugins/freetype-2.6/lib
)

set(MAGIC_MATH_FILES
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Math/MagicMath.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Math/MagicMath.cpp)

set(MAGIC_RENDER_FILES
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/Magic_Fonts.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicAlphaWindow.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicCamera.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicEngineAPI.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicEngineContext.h

${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicMessage.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicModelObject.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicPen.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicShader.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicTexture.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicVertex.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicWorld.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/Magic_Fonts.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicAlphaWindow.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicCamera.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicEngineAPI.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicEngineContext.cpp

${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicMessage.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicModelObject.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicPen.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicShader.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicTexture.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicVertex.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicWorld.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicEarth.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicEarth.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicScene.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicScene.h)

set(MAGIC_SYSTEM_FILES
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/System/Supervisor.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/System/Supervisor.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/System/Element.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/System/Element.h)

set(MAGIC_PHYSICAL_FILES
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Physical/Collision.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Physical/Collision.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Physical/MagicFrustum.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Physical/MagicFrustum.cpp)

set(3RDPARTY_ENTITYX_FILES 
${PROJECT_SOURCE_DIR}../../../Plugins/EntityX/config.h
${PROJECT_SOURCE_DIR}../../../Plugins/EntityX/Entity.h
${PROJECT_SOURCE_DIR}../../../Plugins/EntityX/Entity.cc
${PROJECT_SOURCE_DIR}../../../Plugins/EntityX/Entity.h
${PROJECT_SOURCE_DIR}../../../Plugins/EntityX/Entity.inl
${PROJECT_SOURCE_DIR}../../../Plugins/EntityX/EntityX.h
${PROJECT_SOURCE_DIR}../../../Plugins/EntityX/Event.cc
${PROJECT_SOURCE_DIR}../../../Plugins/EntityX/Event.h
${PROJECT_SOURCE_DIR}../../../Plugins/EntityX/quick.h
${PROJECT_SOURCE_DIR}../../../Plugins/EntityX/System.cc
${PROJECT_SOURCE_DIR}../../../Plugins/EntityX/System.h
${PROJECT_SOURCE_DIR}../../../Plugins/EntityX/COPYING)

set(3RDPARTY_ENTITYX_HELP_FILES 
${PROJECT_SOURCE_DIR}../../../Plugins/EntityX/help/Pool.h
${PROJECT_SOURCE_DIR}../../../Plugins/EntityX/help/Pool.cc
${PROJECT_SOURCE_DIR}../../../Plugins/EntityX/help/NonCopyable.h)

set(MAGIC_DEFINE_FILES
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Define/MagicType.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Define/Magic_Macro.h)

IF(WIN32)
	SOURCE_GROUP("Engine\\Render" FILES ${MAGIC_RENDER_FILES})
	SOURCE_GROUP("Math" FILES ${MAGIC_MATH_FILES})
	SOURCE_GROUP("Engine\\System" FILES ${MAGIC_SYSTEM_FILES})
	SOURCE_GROUP("Engine\\Physical" FILES ${MAGIC_PHYSICAL_FILES})
	SOURCE_GROUP("Engine\\Define" FILES ${MAGIC_DEFINE_FILES})
	SOURCE_GROUP("3rdparty\\Entityx" FILES ${3RDPARTY_ENTITYX_FILES})
	SOURCE_GROUP("3rdparty\\Entityx\\help" FILES ${3RDPARTY_ENTITYX_HELP_FILES})
ENDIF(WIN32)

link_libraries(glew32.lib opengl32.lib 
	debug MagicThread_d.lib optimized MagicThread.lib)
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
add_library (${NOW_PROJECT_NAME} SHARED ${DIR_LIB_SRCS} 
${MAGIC_MATH_FILES}
${MAGIC_RENDER_FILES}
${MAGIC_SYSTEM_FILES}
${MAGIC_PHYSICAL_FILES}
${MAGIC_DEFINE_FILES}
${3RDPARTY_ENTITYX_FILES}
${3RDPARTY_ENTITYX_HELP_FILES})

#RUNTIME	.exe、.dll	可执行档
SET_TARGET_PROPERTIES(${NOW_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../../../bin/${CMAKE_SYSTEM_NAME})
#LIBRARY		.so
#SET_TARGET_PROPERTIES(${NOW_PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../../../lib)
#ARCHIVE	.a、.lib包含 dll 的连结介面	.a
SET_TARGET_PROPERTIES(${NOW_PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ../../../lib/${CMAKE_SYSTEM_NAME})