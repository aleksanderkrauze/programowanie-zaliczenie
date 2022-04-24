cmake_minimum_required(VERSION 3.11)
project("Simulation of a pandemic")

# ###############################################
# C++ stuff
# ###############################################
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

# ###############################################
# CMake stuff
# ###############################################
set(BIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/target")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${BIN_DIR}/debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${BIN_DIR}/release")

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
		"python3.6m"
	)
else()
	find_package(Python3 COMPONENTS Development NumPy REQUIRED)

	set(LOCAL_PYTHON_INCLUDE_DIRS
		${Python3_INCLUDE_DIRS}
	)
	set(LOCAL_PYTHON_LIBS
		${Python3_LIBRARIES}
	)
endif()

# ###############################################
# defining targets names, sources, include directories, etc.
# ###############################################
set(SRC_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(TESTS_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/tests")
set(3RD_PARTY_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party")

set(TARGET_BIN_NAME simulation)
set(TARGET_LIB_NAME simulationlib)
set(TARGET_TESTS_NAME tests)
set(TCLAP_LIB_NAME tclap)
set(PYBIND11_LIB_NAME pybind11)
set(MATPLOTLIBCPP17_LIB_NAME matplotlibcpp17)

set(TARGET_BIN_SRC "${SRC_ROOT}/main.cpp")
file(GLOB TARGET_LIB_SRC "${SRC_ROOT}/lib/*.cpp")
set(TARGET_INCLUDE_DIRS "${SRC_ROOT}/include")
file(GLOB TARGET_TEST_SRC "${TESTS_ROOT}/*.cpp")

file(GLOB TCLAP_LIB_SRC "${3RD_PARTY_ROOT}/tclap/*.h")
set(TCLAP_INCLUDE_DIRS "${3RD_PARTY_ROOT}")

file(GLOB PYBIND11_LIB_SRC "${3RD_PARTY_ROOT}/pybind11/*.h")
set(PYBIND11_INCLUDE_DIRS "${3RD_PARTY_ROOT}/pybind11")

file(GLOB MATPLOTLIBCPP17_LIB_SRC "${3RD_PARTY_ROOT}/matplotlibcpp17/*.h")
set(MATPLOTLIBCPP17_INCLUDE_DIRS "${3RD_PARTY_ROOT}")

# ###############################################
# target pybind11
# ###############################################
add_library(${PYBIND11_LIB_NAME} INTERFACE)

target_sources(${PYBIND11_LIB_NAME} INTERFACE
	${PYBIND11_LIB_SRC}
)

target_link_libraries(${PYBIND11_LIB_NAME} INTERFACE
	${LOCAL_PYTHON_LIBS}
)

# ###############################################
# target matplotlibcpp17
# ###############################################
add_library(${MATPLOTLIBCPP17_LIB_NAME} INTERFACE)

target_sources(${MATPLOTLIBCPP17_LIB_NAME} INTERFACE
	${MATPLOTLIBCPP17_LIB_SRC}
)

target_include_directories(${MATPLOTLIBCPP17_LIB_NAME} INTERFACE
	${LOCAL_PYTHON_INCLUDE_DIRS}
	${PYBIND11_INCLUDE_DIRS}
)

target_link_libraries(${MATPLOTLIBCPP17_LIB_NAME} INTERFACE
	${LOCAL_PYTHON_LIBS}
	${PYBIND11_LIB_NAME}
)

# ###############################################
# target tclap
# ###############################################
add_library(${TCLAP_LIB_NAME} INTERFACE)

target_sources(${TCLAP_LIB_NAME} INTERFACE
	${TCLAP_LIB_SRC}
)

# ###############################################
# target main lib
# ###############################################
add_library(${TARGET_LIB_NAME} STATIC
	${TARGET_LIB_SRC}
)

target_include_directories(${TARGET_LIB_NAME} PUBLIC
	${TARGET_INCLUDE_DIRS}
	${PYBIND11_INCLUDE_DIRS}
	${MATPLOTLIBCPP17_INCLUDE_DIRS}
)

target_link_libraries(${TARGET_LIB_NAME} PUBLIC
	${MATPLOTLIBCPP17_LIB_NAME}
)

# ###############################################
# target binary
# ###############################################
add_executable(${TARGET_BIN_NAME}
	${TARGET_BIN_SRC}
)

target_include_directories(${TARGET_BIN_NAME} PUBLIC
	${TARGET_INCLUDE_DIRS}
	${TCLAP_INCLUDE_DIRS}
)

target_link_libraries(${TARGET_BIN_NAME} PUBLIC
	${TARGET_LIB_NAME}
	${TCLAP_LIB_NAME}
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
