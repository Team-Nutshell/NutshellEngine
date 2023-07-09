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
	src/ntshengn_core.cpp)
set(CORE_HEADERS
	src/ntshengn_core.h)

set(MODULE_LOADER_HEADERS
	src/module_loader/ntshengn_module_loader.h)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	list(APPEND MODULE_LOADER_HEADERS
		src/module_loader/ntshengn_module_loader_windows.h)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	list(APPEND MODULE_LOADER_HEADERS
		src/module_loader/ntshengn_module_loader_linux.h)
endif()

set(FRAME_LIMITER_SOURCES
	src/frame_limiter/ntshengn_frame_limiter.cpp)
set(FRAME_LIMITER_HEADERS
	src/frame_limiter/ntshengn_frame_limiter.h)

set(NETWORKING_SOURCES
	src/networking/ntshengn_networking_server_socket.cpp
	src/networking/ntshengn_networking_client_socket.cpp
	src/networking/ntshengn_networking.cpp)
set(NETWORKING_HEADERS
	src/networking/ntshengn_networking_common.h
	src/networking/ntshengn_networking_server_socket.h
	src/networking/ntshengn_networking_client_socket.h
	src/networking/ntshengn_networking.h)

set(SCRIPTING_SOURCES
	src/scripting/ntshengn_scripting.cpp)
set(SCRIPTING_HEADERS
	src/scripting/ntshengn_scripting.h)

set(UTILS_HEADERS
	src/utils/ntshengn_core_defines.h)

set(SOURCES ${CORE_SOURCES} ${FRAME_LIMITER_SOURCES} ${NETWORKING_SOURCES} ${SCRIPTING_SOURCES})
set(HEADERS ${CORE_HEADERS} ${MODULE_LOADER_HEADERS} ${FRAME_LIMITER_HEADERS} ${NETWORKING_HEADERS} ${SCRIPTING_HEADERS} ${UTILS_HEADERS})

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