cmake_minimum_required(VERSION 3.5.1)
project(stoneshire_http_server)

# Init include directories for the compiler
set(STONESHIRE_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)

# Enable unit testing -> it can be done from the ccmake cofiguration panel
option(ENABLE_TESTS "Perform unit tests" ON)
if(ENABLE_TESTS)
	list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)
	find_package(CMocka CONFIG REQUIRED)
	if(CMocka_FOUND)
		include(AddCMockaTest)
		add_subdirectory(test)
		enable_testing()
	else(CMocka_FOUND)
		message(FATAL_ERROR "CMocka package not found.")
	endif(CMocka_FOUND)
endif(ENABLE_TESTS)
