if(CMAKE_CL_64)
set(NOW_x86_x64 "x64")
else(CMAKE_CL_64)
set(NOW_x86_x64 "x86")
endif(CMAKE_CL_64)

execute_process(
COMMAND git clone https://github.com/litehtml/litehtml
WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}../../../Plugins/)

include_directories(
../../../Plugins/glm
../../../Core/Magic_Engine/Include
../../../Core/Magic_Engine/
../../../Core/MagicUI/Include
../../../Core/MagicUI/
../../../Core/WaterMagic
../../../Core/MagicThread
../../../Plugins/Structure
../../../Plugins/litehtml/include
)

link_directories(
${PROJECT_SOURCE_DIR}../../../lib/${CMAKE_SYSTEM_NAME}/${NOW_x86_x64}
${PROJECT_SOURCE_DIR}../../../Plugins/litehtml/Win32)

set(SCENE_FILES
)

set(SOURCE_FILES
${PROJECT_SOURCE_DIR}../../../Core/WaterMagic/Main.cpp
${PROJECT_SOURCE_DIR}../../../Core/WaterMagic/container_test.cpp
${PROJECT_SOURCE_DIR}../../../Core/WaterMagic/container_test.h)

SOURCE_GROUP("Scene" FILES ${SCENE_FILES})

link_libraries(
	litehtml.lib
	debug Magic_Engine_d.lib optimized Magic_Engine.lib
	debug MagicUI_d.lib optimized MagicUI.lib)



IF(WIN32)
	add_definitions(-DUNICODE -D_UNICODE)
	SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS") 
ENDIF(WIN32)

set(NOW_PROJECT_NAME WaterMagic)

ADD_EXECUTABLE (${NOW_PROJECT_NAME} ${SOURCE_FILES} ${SCENE_FILES})

#RUNTIME	.exe、.dll	可执行档
SET_TARGET_PROPERTIES(${NOW_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../../../bin/${CMAKE_SYSTEM_NAME}/${NOW_x86_x64} OUTPUT_NAME_DEBUG ${NOW_PROJECT_NAME}_d)
#LIBRARY		.so
#SET_TARGET_PROPERTIES(${NOW_PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../../../lib)
#ARCHIVE	.a、.lib包含 dll 的连结介面	.a
SET_TARGET_PROPERTIES(${NOW_PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ../../../lib/${CMAKE_SYSTEM_NAME}/${NOW_x86_x64} OUTPUT_NAME_DEBUG ${NOW_PROJECT_NAME}_d)

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
	${PROJECT_SOURCE_DIR}/../../Plugins/lpng/dll/${NOW_x86_x64}/libpng16d.dll
	${PROJECT_SOURCE_DIR}/../../Plugins/lpng/dll/${NOW_x86_x64}/zlib1d.dll
	${PROJECT_SOURCE_DIR}/../../Plugins/glew/bin/${NOW_x86_x64}/glew32.dll
	)
	
	set(DLL_RELEASE_INSTALL_FILES
	${PROJECT_SOURCE_DIR}/../../Plugins/lpng/dll/${NOW_x86_x64}/libpng16.dll
    ${PROJECT_SOURCE_DIR}/../../Plugins/lpng/dll/${NOW_x86_x64}/zlib1.dll
	${PROJECT_SOURCE_DIR}/../../Plugins/glew/bin/${NOW_x86_x64}/glew32.dll
	)
	
	install(FILES ${DLL_DEBUG_INSTALL_FILES} DESTINATION ${PROJECT_SOURCE_DIR}../../../bin/${CMAKE_SYSTEM_NAME}/${NOW_x86_x64}/Debug)
	install(FILES ${DLL_RELEASE_INSTALL_FILES} DESTINATION ${PROJECT_SOURCE_DIR}../../../bin/${CMAKE_SYSTEM_NAME}/${NOW_x86_x64}/Release)
ENDIF(WIN32)