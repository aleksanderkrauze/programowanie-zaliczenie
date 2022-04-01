cmake_minimum_required(VERSION 3.11)
project(Simulation_of_a_pandemic)

# ###############################################
# C++ stuff
# ###############################################
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-O2 -Wall -Wextra")

# ###############################################
# CMake stuff
# ###############################################
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/target")

# ###############################################
# Detecting build environment
# ###############################################
cmake_host_system_information(RESULT HOSTNAME QUERY HOSTNAME)
if(${HOSTNAME} MATCHES "fuw.edu.pl$")
	set(BUILD_ENV_REMOTE TRUE)
else()
	set(BUILD_ENV_REMOTE FALSE)
endif()

# ###############################################
# Find Python libs
# ###############################################
if(BUILD_ENV_REMOTE)
	set(LOCAL_PYTHON_INCLUDE_DIRS
		"/usr/include/python3.6m/"
	)
	set(LOCAL_PYTHON_LIBS
		"python3"
	)
else()
	find_package(Python3 COMPONENTS Development NumPy REQUIRED)

	set(LOCAL_PYTHON_INCLUDE_DIRS
		${Python3_INCLUDE_DIRS}
		${Python3_NumPy_INCLUDE_DIRS}
	)
	set(LOCAL_PYTHON_LIBS
		${Python3_LIBRARIES}
		Python3::NumPy
	)
endif()

# ###############################################
# defining targets names, sources, include directories, etc.
# ###############################################
set(TARGET_BIN_NAME simulation)
set(TARGET_LIB_NAME simulationlib)
set(TARGET_TESTS_NAME tests)
set(MATPLOTLIBCPP_LIB_NAME matplotlibcpp)
set(TARGET_BIN_SRC
	"src/main.cpp"
)
set(TARGET_LIB_SRC
	"src/city.cpp"
	"src/person.cpp"
	"src/plotter.cpp"
	"src/exceptions.cpp"
)
set(TARGET_TEST_SRC
	"tests/person.cpp"
	"tests/city.cpp"
)
set(MATPLOTLIBCPP_LIB_SRC "${CMAKE_CURRENT_SOURCE_DIR}/include/matplotlibcpp.h")
set(TARGET_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include")

# ###############################################
# target matplotlibcpp.h
# ###############################################
add_library(${MATPLOTLIBCPP_LIB_NAME} INTERFACE)

# Divide adding library and specifying it's sources to support older versions of CMake
target_sources(${MATPLOTLIBCPP_LIB_NAME} INTERFACE
	${MATPLOTLIBCPP_LIB_SRC}
)

target_include_directories(${MATPLOTLIBCPP_LIB_NAME} INTERFACE
	${LOCAL_PYTHON_INCLUDE_DIRS}
)

target_link_libraries(${MATPLOTLIBCPP_LIB_NAME} INTERFACE
	${LOCAL_PYTHON_LIBS}
)

# ###############################################
# target main lib
# ###############################################
add_library(${TARGET_LIB_NAME} STATIC
	${TARGET_LIB_SRC}
)

target_include_directories(${TARGET_LIB_NAME} PUBLIC
	${TARGET_INCLUDE_DIRS}
)

target_link_libraries(${TARGET_LIB_NAME} PUBLIC
	${MATPLOTLIBCPP_LIB_NAME}
)

# ###############################################
# target binary
# ###############################################
add_executable(${TARGET_BIN_NAME}
	${TARGET_BIN_SRC}
)

target_include_directories(${TARGET_BIN_NAME} PUBLIC
	${TARGET_INCLUDE_DIRS}
)

target_link_libraries(${TARGET_BIN_NAME} PUBLIC
	${TARGET_LIB_NAME}
)

# ###############################################
# tests
# ###############################################
enable_testing()
find_package(GTest)
if(GTest_FOUND)
	add_executable(${TARGET_TESTS_NAME}
		${TARGET_TEST_SRC}
	)
	target_include_directories(${TARGET_TESTS_NAME} PUBLIC
		${TARGET_INCLUDE_DIRS}
	)
	target_link_libraries(${TARGET_TESTS_NAME}
		${TARGET_LIB_NAME}
		GTest::gtest
		GTest::gtest_main
	)

	add_test(UnitTests ${TARGET_TESTS_NAME})
else()
	message("Google test was not found. Testing is disabled.")
endif()
