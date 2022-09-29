cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
set(NAME NutshellEngine)
project(${NAME} VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NTSH_DEFINES "")
set(NTSH_LIBRARIES "")

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	list(APPEND NTSH_LIBRARIES ${CMAKE_DL_LIBS})
endif()

add_subdirectory(external/Common)

set(UTILS_HEADERS
	src/utils/ntsh_core_defines.h)

set(SOURCES "")
set(HEADERS ${UTILS_HEADERS})

add_executable(${PROJECT_NAME} src/main.cpp ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE NutshellEngine::Common ${NTSH_LIBRARIES})

target_compile_options(${PROJECT_NAME} PRIVATE
	$<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
	$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Werror>)

target_compile_definitions(${PROJECT_NAME} PRIVATE
	${NTSH_DEFINES})