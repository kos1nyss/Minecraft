cmake_minimum_required (VERSION 3.15)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(PROJECT_NAME Minecraft)
	 
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} main.cpp
        	inc/object.h src/object.cpp
		inc/block.h src/block.cpp
		inc/world.h src/world.cpp
		inc/player.h src/player.cpp
		inc/ray.h src/ray.cpp
		inc/perlin_noise.h src/perlin_noise.cpp)


INCLUDE_DIRECTORIES(
    ${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/ext/freeglut/include"
)

LINK_DIRECTORIES(
    ${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/ext/freeglut/lib"
)

INCLUDE_DIRECTORIES("inc/")

TARGET_LINK_LIBRARIES(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/ext/freeglut/lib/freeglut.lib")

add_custom_target(copy-files ALL
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/ext/freeglut/bin/freeglut.dll
    ${CMAKE_CURRENT_BINARY_DIR}/Debug
)