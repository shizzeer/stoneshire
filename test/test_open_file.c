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

void *__real_malloc(size_t size);
void *__wrap_malloc(size_t size) {
	check_expected_ptr(size);

	bool malloc_success = mock_type(bool);

	if (malloc_success) {
		return __real_malloc(size);
	} else {
		return NULL;
	}
}

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

static void test_open_file_success(void **state) {
	expect_any(__wrap_malloc, size);
	will_return(__wrap_malloc, true);

	expect_string(__wrap_fopen, mode, "r");
	will_return(__wrap_fopen, WRAP_FOPEN_CORRECT);

	FILE *config_file = open_file("ports.conf", CONF_DIR, strlen("ports.conf"), strlen(CONF_DIR));
	assert_non_null(config_file);
}

static void test_open_file_failed(void **state) {
	expect_any(__wrap_malloc, size);
	will_return(__wrap_malloc, true);

	expect_string(__wrap_fopen, mode, "r");
	will_return(__wrap_fopen, WRAP_FOPEN_CORRECT);

	FILE *config_file = open_file("wrong_file.conf", CONF_DIR, strlen("wrong_file.conf"), strlen(CONF_DIR));
	assert_null(config_file);
}

int main() {

	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_open_file_success),
		cmocka_unit_test(test_open_file_failed)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}