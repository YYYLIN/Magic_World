set(NOW_PROJECT_NAME MagicGrating)

if(CMAKE_CL_64)
set(NOW_x86_x64 "x64")
else(CMAKE_CL_64)
set(NOW_x86_x64 "x86")
endif(CMAKE_CL_64)

include_directories(
../../../Plugins/Structure
../../../Core/MagicGrating/MagicGrating
../../../Core/MagicGrating)

set(INCLUDE_FILES
${PROJECT_SOURCE_DIR}../../../Core/MagicGrating/MagicGrating/MagicError.h
${PROJECT_SOURCE_DIR}../../../Core/MagicGrating/MagicGrating/MagicGratingAPI.h
${PROJECT_SOURCE_DIR}../../../Core/MagicGrating/MagicGrating/MagicMath.hpp
${PROJECT_SOURCE_DIR}../../../Core/MagicGrating/MagicGrating/MagicMath.inl
${PROJECT_SOURCE_DIR}../../../Core/MagicGrating/MagicGrating/MagicStruct.h)

set(SOURCE_FILES
${PROJECT_SOURCE_DIR}../../../Core/MagicGrating/DynamicQueue.hpp
${PROJECT_SOURCE_DIR}../../../Core/MagicGrating/DynamicQueue.inl
${PROJECT_SOURCE_DIR}../../../Core/MagicGrating/MagicFrameBufferObject.cpp
${PROJECT_SOURCE_DIR}../../../Core/MagicGrating/MagicFrameBufferObject.h
${PROJECT_SOURCE_DIR}../../../Core/MagicGrating/MagicGlobalVariable.cpp
${PROJECT_SOURCE_DIR}../../../Core/MagicGrating/MagicGlobalVariable.h
${PROJECT_SOURCE_DIR}../../../Core/MagicGrating/MagicGratingAPI.cpp
${PROJECT_SOURCE_DIR}../../../Core/MagicGrating/MagicInsideMacro.h
${PROJECT_SOURCE_DIR}../../../Core/MagicGrating/MagicList.hpp
${PROJECT_SOURCE_DIR}../../../Core/MagicGrating/MagicList.inl
${PROJECT_SOURCE_DIR}../../../Core/MagicGrating/MagicShaderObject.cpp
${PROJECT_SOURCE_DIR}../../../Core/MagicGrating/MagicShaderObject.h
${PROJECT_SOURCE_DIR}../../../Core/MagicGrating/MagicVertexBufferObject.cpp
${PROJECT_SOURCE_DIR}../../../Core/MagicGrating/MagicVertexBufferObject.h)

SOURCE_GROUP("Include Files" FILES ${INCLUDE_FILES})
SOURCE_GROUP("Source Files" FILES ${SOURCE_FILES})

add_definitions(-DMAGICGRATING_EXPORTS)
IF(WIN32)
	add_definitions(-DUNICODE -D_UNICODE)
ENDIF(WIN32)

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(../../../Core/MagicGrating DIR_LIB_SRCS)

add_library (MagicGrating SHARED ${DIR_LIB_SRCS} ${INCLUDE_FILES} ${SOURCE_FILES})

#RUNTIME	.exe、.dll	可执行档
SET_TARGET_PROPERTIES(MagicGrating PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../../../bin/${CMAKE_SYSTEM_NAME}/${NOW_x86_x64} OUTPUT_NAME_DEBUG ${NOW_PROJECT_NAME}_d)
#LIBRARY		.so
#SET_TARGET_PROPERTIES(MagicGrating PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../../../lib)
#ARCHIVE	.a、.lib包含 dll 的连结介面	.a
SET_TARGET_PROPERTIES(MagicGrating PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ../../../lib/${CMAKE_SYSTEM_NAME}/${NOW_x86_x64} OUTPUT_NAME_DEBUG ${NOW_PROJECT_NAME}_d)