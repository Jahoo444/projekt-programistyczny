cmake_minimum_required(VERSION 3.0)
project(simulation)

set(SDL2_INCLUDE_DIR /usr/include/SDL2)
set(SDL2_LIBRARY /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.4.0)

include_directories(${bt}/simulation)
include_directories(${SDL2_INCLUDE_DIR})

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/simulation)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 14)
SET(SDL_LINK_FLAGS "-lSDL2")
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${SDL_LINK_FLAGS}" )

set(SOURCES
	simulation/main.cpp
	simulation/Car.cpp
	simulation/Renderer.cpp
	simulation/Simulation.cpp
	simulation/SimulationState.cpp
	simulation/TileMap.cpp
)

set(HEADERS
	simulation/Camera.h
	simulation/Car.h
	simulation/Light.h
	simulation/Renderer.h
	simulation/Simulation.h
	simulation/SimulationState.h
	simulation/State.h
	simulation/TileMap.h
)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
