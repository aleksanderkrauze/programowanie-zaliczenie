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
set(TCLAP_LIB_NAME tclap)

set(TARGET_BIN_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
file(GLOB TARGET_LIB_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/lib/*.cpp")
set(TARGET_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/include")
file(GLOB TARGET_TEST_SRC "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")

set(MATPLOTLIBCPP_LIB_SRC "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/matplotlibcpp/matplotlibcpp.h")
set(MATPLOTLIBCPP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/matplotlibcpp")

file(GLOB TCLAP_LIB_SRC "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/tclap/*.h")
set(TCLAP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/")

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
	${MATPLOTLIBCPP_INCLUDE_DIRS}
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
