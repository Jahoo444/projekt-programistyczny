cmake_minimum_required(VERSION 3.7)

project(simulation)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

set(SOURCES
	src/main.cpp
	src/Car.cpp
	src/Renderer.cpp
	src/Simulation.cpp
	src/SimulationState.cpp
	src/TileMap.cpp
)

set(HEADERS
	src/Camera.h
	src/Car.h
	src/Light.h
	src/Renderer.h
	src/Simulation.h
	src/SimulationState.h
	src/State.h
	src/TileMap.h
)


add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} )
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
