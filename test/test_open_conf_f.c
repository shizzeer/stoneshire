#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "err_handlers.h"
#include "conf.h"

#define WRAP_FOPEN_CORRECT 0x11111111

struct conf config;

FILE *__wrap_fopen(const char *path, const char *mode) {
	/* Checks only one of the parameters because set_config_file_path function
	   changes the value of path and it's hard to check this parameter inside the 
	   test.*/
	check_expected_ptr(mode);

	if (!access(path, R_OK)) {
		return mock_type(FILE *);
	} else {
		mock_type(FILE *);
		return NULL;
	}
}

static void test_open_conf_f_success(void **state) {
	expect_string(__wrap_fopen, mode, "r");

	will_return(__wrap_fopen, WRAP_FOPEN_CORRECT);

	FILE *config_file = open_config_file("ports.conf", strlen("ports.conf"), &config);
	assert_non_null(config_file);
}

static void test_open_conf_f_failed(void **state) {
	expect_string(__wrap_fopen, mode, "r");

	will_return(__wrap_fopen, WRAP_FOPEN_CORRECT);

	FILE *config_file = open_config_file("wrong_file.conf", strlen("wrong_file.conf"), &config);
	assert_null(config_file);
}

void preparing_to_test(struct conf *config) {
	init_default_conf_dir("/home/shizzer/c/stoneshire/conf/", config);
}

int main() {
	preparing_to_test(&config);

	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_open_conf_f_success),
		cmocka_unit_test(test_open_conf_f_failed)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}