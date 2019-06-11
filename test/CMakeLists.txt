# Set appropriate cmocka library to do unit testing
set(CMOCKA_LIBRARY CACHE PATH "/usr/local/lib/libcmocka.so")

set(CONFIG_TEST ${CMAKE_SOURCE_DIR}/conf/conf.c)
set(ERR_HANDLERS ${CMAKE_SOURCE_DIR}/src/err_handlers.c)

include_directories(${CMAKE_SOURCE_DIR}/include)
find_library(CMOCKA_LIB cmocka HINTS /usr/local/lib/)

add_cmocka_test(test_open_conf_f 
				SOURCES test_open_conf_f.c ${CONFIG_TEST} ${ERR_HANDLERS} 
				LINK_LIBRARIES ${CMOCKA_LIB}
				LINK_OPTIONS "-Wl,--wrap=fopen")
