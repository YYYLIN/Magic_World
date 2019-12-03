if(CMAKE_CL_64)
set(NOW_x86_x64 "x64")
else(CMAKE_CL_64)
set(NOW_x86_x64 "x86")
endif(CMAKE_CL_64)

include_directories(
../../../Plugins/Structure
../../../Plugins/lpng/Include
../../../Plugins/glm
../../../Plugins/glew/include
../../../Plugins/freetype-2.6/include
../../../Core/Magic_Engine
../../../Core/MagicThread)

link_directories(
${PROJECT_SOURCE_DIR}../../../lib/${CMAKE_SYSTEM_NAME}/${NOW_x86_x64}
${PROJECT_SOURCE_DIR}../../../Plugins/freetype-2.6/lib
)

link_directories(
${PROJECT_SOURCE_DIR}../../../Plugins/glew/lib/${NOW_x86_x64}
${PROJECT_SOURCE_DIR}../../../Plugins/lpng/lib/${NOW_x86_x64}
)

set(MAGIC_MATH_FILES
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Math/MagicMath.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Math/MagicMath.cpp)

set(MAGIC_RENDER_FILES
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicCamera.h

${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicPen.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicShader.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicTexture.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicVertex.h

${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicCamera.cpp

${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicPen.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicShader.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicTexture.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicVertex.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/Render_thread.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/Render_thread.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/DrawSimpleGraphics.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/DrawSimpleGraphics.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicOpengl.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicOpengl.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/TemplateEffects.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/TemplateEffects.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/DrawTool.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/DrawTool.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MainTemplateEffects.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MainTemplateEffects.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicFBO.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/MagicFBO.h)

set(MAGIC_RENDER_EFFECTS_FILES
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/Effects/FuzzyEffect.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Render/Effects/FuzzyEffect.cpp)

set(MAGIC_SYSTEM_FILES
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/System/MagicWorld.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/System/MagicWorld.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/System/MagicEngineContext.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/System/MagicEngineContext.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/System/MagicEngineAPI.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/System/MagicEngineAPI.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/System/MagicEarth.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/System/MagicEarth.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/System/MagicModelObject.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/System/MagicModelObject.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/System/Magic_Fonts.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/System/Magic_Fonts.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/System/MagicAlphaWindow.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/System/MagicAlphaWindow.cpp
)

set(MAGIC_PHYSICAL_FILES
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Physical/Collision.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Physical/Collision.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Physical/MagicFrustum.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Physical/MagicFrustum.cpp)

set(MAGIC_DEFINE_FILES
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Define/MagicType.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Define/Magic_Macro.h)

set(MAGIC_TOOL_FILES
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Tool/VariableMonitoring.hpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Tool/InjectFunction.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Tool/InjectFunction.cpp
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Tool/GlobalStaticMemory.h
${PROJECT_SOURCE_DIR}../../../Core/Magic_Engine/Tool/GlobalStaticMemory.cpp)

IF(WIN32)
	SOURCE_GROUP("Engine\\Render" FILES ${MAGIC_RENDER_FILES})
		SOURCE_GROUP("Engine\\Render\\Effects" FILES ${MAGIC_RENDER_EFFECTS_FILES})
	SOURCE_GROUP("Engine\\Math" FILES ${MAGIC_MATH_FILES})
	SOURCE_GROUP("Engine\\System" FILES ${MAGIC_SYSTEM_FILES})
	SOURCE_GROUP("Engine\\Physical" FILES ${MAGIC_PHYSICAL_FILES})
	SOURCE_GROUP("Engine\\Define" FILES ${MAGIC_DEFINE_FILES})
	SOURCE_GROUP("Engine\\Tool" FILES ${MAGIC_TOOL_FILES})
ENDIF(WIN32)

link_libraries(glew32.lib opengl32.lib
	debug libpngd.lib optimized libpng.lib
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
${MAGIC_RENDER_EFFECTS_FILES}
${MAGIC_SYSTEM_FILES}
${MAGIC_PHYSICAL_FILES}
${MAGIC_DEFINE_FILES}
${MAGIC_TOOL_FILES})

#RUNTIME	.exe、.dll	可执行档
SET_TARGET_PROPERTIES(${NOW_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../../../bin/${CMAKE_SYSTEM_NAME}/${NOW_x86_x64} OUTPUT_NAME_DEBUG ${NOW_PROJECT_NAME}_d)
#LIBRARY		.so
#SET_TARGET_PROPERTIES(${NOW_PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../../../lib)
#ARCHIVE	.a、.lib包含 dll 的连结介面	.a
SET_TARGET_PROPERTIES(${NOW_PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ../../../lib/${CMAKE_SYSTEM_NAME}/${NOW_x86_x64} OUTPUT_NAME_DEBUG ${NOW_PROJECT_NAME}_d)

add_dependencies(${NOW_PROJECT_NAME} MagicThread)