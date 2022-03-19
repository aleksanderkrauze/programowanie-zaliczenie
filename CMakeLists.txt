cmake_minimum_required(VERSION 3.11)

project(Simulation_of_a_pandemic)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Python3 COMPONENTS Development NumPy REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/target")

set(TARGET_BIN_NAME simulation)
set(TARGET_LIB_NAME simulationlib)
set(MATPLOTLIBCPP_LIB_NAME matplotlibcpp)
set(TARGET_BIN_SRC
	"src/main.cpp"
)
set(TARGET_LIB_SRC
	"src/city.cpp"
	"src/person.cpp"
	"src/plotter.cpp"
)
set(TARGET_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include")

# matplotlibcpp.h
add_library(${MATPLOTLIBCPP_LIB_NAME} INTERFACE
	"include/matplotlibcpp.h"
)

target_include_directories(${MATPLOTLIBCPP_LIB_NAME} INTERFACE
	${Python3_INCLUDE_DIRS}
	${Python3_NumPy_INCLUDE_DIRS}
)

target_link_libraries(${MATPLOTLIBCPP_LIB_NAME} INTERFACE
	${Python3_LIBRARIES}
	Python3::NumPy
)

# main lib
add_library(${TARGET_LIB_NAME} STATIC
	${TARGET_LIB_SRC}
)

target_include_directories(${TARGET_LIB_NAME} PUBLIC
	${TARGET_INCLUDE_DIRS}
)

target_link_libraries(${TARGET_LIB_NAME} PUBLIC
	${MATPLOTLIBCPP_LIB_NAME}
)

# binary
add_executable(${TARGET_BIN_NAME}
	${TARGET_BIN_SRC}
)

target_include_directories(${TARGET_BIN_NAME} PUBLIC
	${TARGET_INCLUDE_DIRS}
)

target_link_libraries(${TARGET_BIN_NAME} PUBLIC
	${TARGET_LIB_NAME}
)