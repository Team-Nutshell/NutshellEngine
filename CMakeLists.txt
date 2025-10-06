cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
set(NAME NutshellEngine)
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

set(ASSET_MANAGER_SOURCES
	asset_manager/ntshengn_asset_manager.cpp)
set(ASSET_MANAGER_HEADERS
	asset_manager/ntshengn_asset_manager.h)

set(COMMAND_LINE_SOURCES
	command_line/ntshengn_command_line.cpp)
set(COMMAND_LINE_HEADERS
	command_line/ntshengn_command_line.h)

set(CORE_SOURCES
	ntshengn_core.cpp)
set(CORE_HEADERS
	ntshengn_core.h)

set(ECS_SOURCES
	ecs/ntshengn_ecs.cpp)
set(ECS_HEADERS
	ecs/ntshengn_ecs.h)
	
set(EXCEPTION_HANDLER_SOURCES "")
set(EXCEPTION_HANDLER_HEADERS
	exception_handler/ntshengn_exception_handler.h)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	list(APPEND EXCEPTION_HANDLER_SOURCES
	exception_handler/ntshengn_exception_handler_windows.cpp)
	list(APPEND EXCEPTION_HANDLER_HEADERS
		exception_handler/ntshengn_exception_handler_windows.h)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
	list(APPEND EXCEPTION_HANDLER_SOURCES
		exception_handler/ntshengn_exception_handler_linux_freebsd.cpp)
	list(APPEND EXCEPTION_HANDLER_HEADERS
		exception_handler/ntshengn_exception_handler_linux_freebsd.h)
endif()

set(FRAME_LIMITER_SOURCES
	frame_limiter/ntshengn_frame_limiter.cpp)
set(FRAME_LIMITER_HEADERS
	frame_limiter/ntshengn_frame_limiter.h)

set(JOB_SYSTEM_SOURCES
	job_system/ntshengn_job_system.cpp)
set(JOB_SYSTEM_HEADERS
	job_system/ntshengn_job_system.h)

set(MODULE_LOADER_HEADERS
	module_loader/ntshengn_module_loader.h)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	list(APPEND MODULE_LOADER_HEADERS
		module_loader/ntshengn_module_loader_windows.h)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
	list(APPEND MODULE_LOADER_HEADERS
		module_loader/ntshengn_module_loader_linux_freebsd.h)
endif()

set(NETWORKING_SOURCES
	networking/ntshengn_networking.cpp
	networking/ntshengn_networking_client_socket.cpp
	networking/ntshengn_networking_server_socket.cpp)
set(NETWORKING_HEADERS
	networking/ntshengn_networking.h
	networking/ntshengn_networking_client_socket.h
	networking/ntshengn_networking_server_socket.h)

set(PROFILER_SOURCES
	profiler/ntshengn_profiler.cpp)
set(PROFILER_HEADERS
	profiler/ntshengn_profiler.h)

set(SCENE_MANAGER_SOURCES
	scene_manager/ntshengn_scene_manager.cpp)
set(SCENE_MANAGER_HEADERS
	scene_manager/ntshengn_scene_manager.h)

set(SCRIPT_MANAGER_LOADER_HEADERS
	script_manager_loader/ntshengn_script_manager_loader.h)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	list(APPEND SCRIPT_MANAGER_LOADER_HEADERS
		script_manager_loader/ntshengn_script_manager_loader_windows.h)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
	list(APPEND SCRIPT_MANAGER_LOADER_HEADERS
		script_manager_loader/ntshengn_script_manager_loader_linux_freebsd.h)
endif()

set(SCRIPTING_SOURCES
	scripting/ntshengn_scripting.cpp)
set(SCRIPTING_HEADERS
	scripting/ntshengn_scripting.h)

set(UTILS_HEADERS
	utils/ntshengn_core_defines.h)

set(SOURCES ${ASSET_MANAGER_SOURCES} ${COMMAND_LINE_SOURCES} ${CORE_SOURCES} ${ECS_SOURCES} ${EXCEPTION_HANDLER_SOURCES} ${FRAME_LIMITER_SOURCES} ${JOB_SYSTEM_SOURCES} ${NETWORKING_SOURCES} ${PROFILER_SOURCES} ${SCENE_MANAGER_SOURCES} ${SCRIPTING_SOURCES})
set(HEADERS ${ASSET_MANAGER_HEADERS} ${COMMAND_LINE_HEADERS} ${CORE_HEADERS} ${ECS_HEADERS} ${EXCEPTION_HANDLER_HEADERS} ${FRAME_LIMITER_HEADERS} ${JOB_SYSTEM_HEADERS} ${MODULE_LOADER_HEADERS} ${NETWORKING_HEADERS} ${PROFILER_HEADERS} ${SCENE_MANAGER_HEADERS} ${SCRIPT_MANAGER_LOADER_HEADERS} ${SCRIPTING_HEADERS} ${UTILS_HEADERS})

add_executable(${PROJECT_NAME} main.cpp ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME}
	PUBLIC
		NutshellEngine::Common
		${NTSHENGN_LIBRARIES})

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	target_link_libraries(${PROJECT_NAME}
		PRIVATE
		Dbghelp.lib)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
	set(THREADS_PREFER_PTHREAD_FLAG TRUE)
	find_package(Threads REQUIRED)
	target_link_libraries(${PROJECT_NAME}
		PRIVATE
			Threads::Threads)
endif()

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

set_target_properties(${PROJECT_NAME}
	PROPERTIES
	VS_DEBUGGER_WORKING_DIRECTORY "$(OutDir)")