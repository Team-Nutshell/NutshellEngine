cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
set(NAME NutshellEngine-Core)
project(${NAME} VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NTSHENGN_DEFINES "")
set(NTSHENGN_LIBRARIES "")

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	list(APPEND NTSHENGN_LIBRARIES ${CMAKE_DL_LIBS})
endif()

set(NTSHENGN_COMMON_PATH "NtshEngn_Common_NOTFOUND" CACHE PATH "The directory containing NutshellEngine's common resources.")
if(NOT ${NTSHENGN_COMMON_PATH} STREQUAL "NtshEngn_Common_NOTFOUND" AND EXISTS ${NTSHENGN_COMMON_PATH})
	file(COPY ${NTSHENGN_COMMON_PATH}/ DESTINATION ${CMAKE_SOURCE_DIR}/external/Common)
	message(STATUS "Copy NutshellEngine's common resources from ${NTSHENGN_COMMON_PATH} to ${CMAKE_SOURCE_DIR}/external/Common")
else()
	message(FATAL_ERROR "Could not find the directory containing NutshellEngine's common resources.")
endif()

add_subdirectory(external/Common)

set(CORE_SOURCES
	src/ntshengn_core.cpp)
set(CORE_HEADERS
	src/ntshengn_core.h)

set(MODULE_LOADER_HEADERS "")
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(MODULE_LOADER_HEADERS
		src/module_loader/ntshengn_module_loader_windows.h)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(MODULE_LOADER_HEADERS
		src/module_loader/ntshengn_module_loader_linux.h)
endif()

set(SCRIPTING_SOURCES
	src/scripting/ntshengn_scripting.cpp)
set(SCRIPTING_HEADERS
	src/scripting/ntshengn_scripting.h)

set(UTILS_HEADERS
	src/utils/ntshengn_core_defines.h)

set(SOURCES ${CORE_SOURCES} ${SCRIPTING_SOURCES})
set(HEADERS ${CORE_HEADERS} ${MODULE_LOADER_HEADERS} ${SCRIPTING_HEADERS} ${UTILS_HEADERS})

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PUBLIC NutshellEngine::Common ${NTSHENGN_LIBRARIES})

target_compile_options(${PROJECT_NAME} PUBLIC
	$<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
	$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Werror>)

target_compile_definitions(${PROJECT_NAME} PUBLIC
	${NTSHENGN_DEFINES}
	PUBLIC
	$<$<CONFIG:Debug>:NTSHENGN_DEBUG>
	$<$<CONFIG:MinSizeRel>:NTSHENGN_RELEASE>
	$<$<CONFIG:Release>:NTSHENGN_RELEASE>
	$<$<CONFIG:RelWithDebInfo>:NTSHENGN_DEBUG>)