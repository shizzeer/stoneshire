option(ENABLE_SRC_COMPILATION "Take files from src to the compilation process." ON)

# Initialize directories with include files to inform the compiler
set(STONESHIRE_INCLUDE_DIRS CACHE STRING ${CMAKE_SOURCE_DIR}/include)

# Initialize variable with necessary configuration file
set(STONESHIRE_CONFIG_FILE ${CMAKE_SOURCE_DIR}/conf/conf.c)

#Initialize variable with necessary log file
set(STONESHIRE_LOG_FILE ${CMAKE_SOURCE_DIR}/log/log.c)

if (ENABLE_SRC_COMPILATION)
	file(GLOB SOURCES "*.c")
	list(APPEND SOURCES ${STONESHIRE_CONFIG_FILE} ${STONESHIRE_LOG_FILE})
	include_directories(${STONESHIRE_INCLUDE_DIRS})
	add_executable(stoneshire ${SOURCES})
endif(ENABLE_SRC_COMPILATION)

