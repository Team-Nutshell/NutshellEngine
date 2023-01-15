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

set(ASSET_LOADER_SOURCES
	src/asset_loader/ntshengn_asset_loader.cpp)
set(ASSET_LOADER_HEADERS
	src/asset_loader/ntshengn_asset_loader.h)

set(UTILS_HEADERS
	src/utils/ntshengn_core_defines.h)

set(SOURCES ${CORE_SOURCES} ${ASSET_LOADER_SOURCES})
set(HEADERS ${CORE_HEADERS} ${ASSET_LOADER_HEADERS} ${MODULE_LOADER_HEADERS} ${UTILS_HEADERS})

add_executable(${PROJECT_NAME} src/main.cpp ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE NutshellEngine::Common ${NTSHENGN_LIBRARIES})

target_compile_options(${PROJECT_NAME} PRIVATE
	$<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
	$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Werror>)

target_compile_definitions(${PROJECT_NAME} PRIVATE
	${NTSHENGN_DEFINES}
	PUBLIC
	$<$<CONFIG:Debug>:NTSHENGN_DEBUG>
	$<$<CONFIG:MinSizeRel>:NTSHENGN_RELEASE>
	$<$<CONFIG:Release>:NTSHENGN_RELEASE>
	$<$<CONFIG:RelWithDebInfo>:NTSHENGN_DEBUG>)