include_directories(
../../../Plugins/glm
../../../Plugins/EntityX
../../../Core/Magic_Engine/Include
../../../Core/Magic_Engine/
../../../Core/MagicUI/Include
../../../Core/MagicUI/
../../../Core/WaterMagic
../../../Plugins/Structure)

link_directories(
${PROJECT_SOURCE_DIR}../../../lib/${CMAKE_SYSTEM_NAME}/)

set(SCENE_FILES
${PROJECT_SOURCE_DIR}../../../Core/WaterMagic/MainScene.cpp
${PROJECT_SOURCE_DIR}../../../Core/WaterMagic/MainScene.h)

SOURCE_GROUP("Scene" FILES ${SCENE_FILES})

link_libraries(Magic_Engine.lib MagicUI.lib)

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(../../../Core/WaterMagic DIR_LIB_SRCS)

IF(WIN32)
	add_definitions(-DUNICODE -D_UNICODE)
	SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS") 
ENDIF(WIN32)

set(NOW_PROJECT_NAME WaterMagic)

ADD_EXECUTABLE (${NOW_PROJECT_NAME} ${DIR_LIB_SRCS} ${SCENE_FILES})

#RUNTIME	.exe、.dll	可执行档
SET_TARGET_PROPERTIES(${NOW_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../../../bin/${CMAKE_SYSTEM_NAME})
#LIBRARY		.so
#SET_TARGET_PROPERTIES(${NOW_PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../../../lib)
#ARCHIVE	.a、.lib包含 dll 的连结介面	.a
SET_TARGET_PROPERTIES(${NOW_PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ../../../lib/${CMAKE_SYSTEM_NAME})

add_dependencies(${NOW_PROJECT_NAME} Magic_Engine)
add_dependencies(${NOW_PROJECT_NAME} MagicUI)


#手动根据已经文件生成vs用户自定义工作路径
file(READ "perconfig.vcxproj.user.in" _perconfig)
set(USERFILE_CONFIGSECTIONS)
# 对于每一个配置（Debug, Release, RelWithDebInfo, MinSizeRel），生成对应的配置项
foreach(USERFILE_CONFIGNAME ${CMAKE_CONFIGURATION_TYPES})
    string(CONFIGURE "${_perconfig}" _temp @ONLY ESCAPE_QUOTES)
    string(CONFIGURE
        "${USERFILE_CONFIGSECTIONS}${_temp}"
        USERFILE_CONFIGSECTIONS
        ESCAPE_QUOTES)
endforeach()

configure_file("vcxproj.user.in"
    ${NOW_PROJECT_NAME}.vcxproj.user
    @ONLY)
	
	
IF(WIN32)
	set(DLL_DEBUG_INSTALL_FILES
	${PROJECT_SOURCE_DIR}/../../Plugins/lpng1621/dll/Debug/libpng16.dll
	${PROJECT_SOURCE_DIR}/../../Plugins/glew-1.9.0/dll/glew32.dll
	)
	
	set(DLL_RELEASE_INSTALL_FILES
	${PROJECT_SOURCE_DIR}/../../Plugins/lpng1621/dll/Release/libpng16.dll
	${PROJECT_SOURCE_DIR}/../../Plugins/glew-1.9.0/dll/glew32.dll
	)
	
	install(FILES ${DLL_DEBUG_INSTALL_FILES} DESTINATION ${PROJECT_SOURCE_DIR}../../../bin/${CMAKE_SYSTEM_NAME}/Debug)
	install(FILES ${DLL_RELEASE_INSTALL_FILES} DESTINATION ${PROJECT_SOURCE_DIR}../../../bin/${CMAKE_SYSTEM_NAME}/Release)
ENDIF(WIN32)