cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
SET(NAME NutshellEngine)
PROJECT(${NAME} VERSION 0.0.1 LANGUAGES C CXX)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/Common)

SET(UTILS_HEADERS
	src/utils/ntsh_core_defines.h)

SET(SOURCES "")
SET(HEADERS ${UTILS_HEADERS})

add_executable(${PROJECT_NAME} src/main.cpp ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE NutshellEngine::Common)

target_compile_options(${PROJECT_NAME} PRIVATE
	$<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
	$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Werror>)