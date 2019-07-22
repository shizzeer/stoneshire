# Set appropriate cmocka library to do unit testing
set(CMOCKA_LIBRARY CACHE PATH "/usr/local/lib/libcmocka.so")

set(CONFIG_TEST ${CMAKE_SOURCE_DIR}/conf/conf.c)
set(ERR_HANDLERS ${CMAKE_SOURCE_DIR}/src/err_handlers.c)

include_directories(${CMAKE_SOURCE_DIR}/include)
find_library(CMOCKA_LIB cmocka HINTS /usr/local/lib/cmocka-1.1.5/build/src/)

add_cmocka_test(test_open_file 
				SOURCES test_open_file.c ${CONFIG_TEST} ${ERR_HANDLERS} 
				LINK_LIBRARIES ${CMOCKA_LIB}
				LINK_OPTIONS "-Wl,--wrap=fopen,--wrap=malloc")

add_cmocka_test(test_fset_conf_state
				SOURCES test_fset_conf_state.c ${CONFIG_TEST} ${ERR_HANDLERS}
				LINK_LIBRARIES ${CMOCKA_LIB}
				LINK_OPTIONS "-Wl,--wrap=getline,--wrap=__isoc99_sscanf,--wrap=strcmp")
