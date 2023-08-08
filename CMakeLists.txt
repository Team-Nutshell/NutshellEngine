cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
set(NAME NutshellEngine-Core)
project(${NAME} VERSION 0.0.1 LANGUAGES C CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NTSHENGN_DEFINES "")
set(NTSHENGN_LIBRARIES "")

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	list(APPEND NTSHENGN_LIBRARIES ${CMAKE_DL_LIBS})
endif()

set(NTSHENGN_COMMON_PATH "NtshEngn_Common_NOTFOUND" CACHE PATH "The directory containing NutshellEngine's common resources.")
if(NOT ${NTSHENGN_COMMON_PATH} STREQUAL "NtshEngn_Common_NOTFOUND" AND EXISTS ${NTSHENGN_COMMON_PATH})
	message(STATUS "Copy NutshellEngine\'s common resources from ${NTSHENGN_COMMON_PATH} to ${CMAKE_CURRENT_SOURCE_DIR}/Common")
	file(COPY ${NTSHENGN_COMMON_PATH}/ DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/Common)
	
	add_subdirectory(Common)
else()
	message(STATUS "Copy NutshellEngine\'s common resources from Git repository to ${CMAKE_CURRENT_SOURCE_DIR}/Common")
	FetchContent_Declare(
		Common
		GIT_REPOSITORY https://github.com/Team-Nutshell/NutshellEngine-Common
		GIT_TAG main
		SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Common"
	)

	FetchContent_MakeAvailable(Common)
endif()

set(CORE_SOURCES
	ntshengn_core.cpp)
set(CORE_HEADERS
	ntshengn_core.h)

set(MODULE_LOADER_HEADERS
	module_loader/ntshengn_module_loader.h)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	list(APPEND MODULE_LOADER_HEADERS
		module_loader/ntshengn_module_loader_windows.h)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	list(APPEND MODULE_LOADER_HEADERS
		module_loader/ntshengn_module_loader_linux.h)
endif()

set(FRAME_LIMITER_SOURCES
	frame_limiter/ntshengn_frame_limiter.cpp)
set(FRAME_LIMITER_HEADERS
	frame_limiter/ntshengn_frame_limiter.h)

set(NETWORKING_SOURCES
	networking/ntshengn_networking_server_socket.cpp
	networking/ntshengn_networking_client_socket.cpp
	networking/ntshengn_networking.cpp)
set(NETWORKING_HEADERS
	networking/ntshengn_networking_common.h
	networking/ntshengn_networking_server_socket.h
	networking/ntshengn_networking_client_socket.h
	networking/ntshengn_networking.h)

set(SCENE_MANAGER_SOURCES
	scene_manager/ntshengn_scene_manager.cpp)
set(SCENE_MANAGER_HEADERS
	scene_manager/ntshengn_scene_manager.h)

set(SCRIPTING_SOURCES
	scripting/ntshengn_scripting.cpp)
set(SCRIPTING_HEADERS
	scripting/ntshengn_scripting.h)

set(UTILS_HEADERS
	utils/ntshengn_core_defines.h)

set(SOURCES ${CORE_SOURCES} ${FRAME_LIMITER_SOURCES} ${NETWORKING_SOURCES} ${SCENE_MANAGER_SOURCES} ${SCRIPTING_SOURCES})
set(HEADERS ${CORE_HEADERS} ${MODULE_LOADER_HEADERS} ${FRAME_LIMITER_HEADERS} ${NETWORKING_HEADERS} ${SCENE_MANAGER_HEADERS} ${SCRIPTING_HEADERS} ${UTILS_HEADERS})

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME}
	PUBLIC
		NutshellEngine::Common
		${NTSHENGN_LIBRARIES})

target_compile_options(${PROJECT_NAME}
	PUBLIC
		$<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
		$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Werror>)

target_compile_definitions(${PROJECT_NAME}
	PUBLIC
		${NTSHENGN_DEFINES}
		$<$<CONFIG:Debug>:NTSHENGN_DEBUG>
		$<$<CONFIG:MinSizeRel>:NTSHENGN_RELEASE>
		$<$<CONFIG:Release>:NTSHENGN_RELEASE>
		$<$<CONFIG:RelWithDebInfo>:NTSHENGN_DEBUG>)