#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdbool.h>
#include <string.h> /* for strlen in open_config_file */

#include "conf.h"

#define LINES_TO_TEST 7

struct conf config;

int __real_getline(char **lineptr, size_t *n, FILE *stream);
int __wrap_getline(char **lineptr, size_t *n, FILE *stream) {
	check_expected_ptr(lineptr);
	check_expected_ptr(n);
	check_expected_ptr(stream);

	bool is_line_in_file = mock_type(bool);

	if (is_line_in_file) {
		return __real_getline(lineptr, n, stream);
	} else {
		return EOF;
	}
}

int __real_strcmp(const char *s1, const char *s2);
int __wrap_strcmp(const char *s1, const char *s2) {
	check_expected_ptr(s1);
	check_expected_ptr(s2);

	bool success = mock_type(bool);

	if (success) {
		return __real_strcmp(s1, s2);
	} else {
		return -1;
	}
}

int __real___isoc99_sscanf(const char *str, const char *format, ...);
int __wrap___isoc99_sscanf(const char *str, const char *format, ...) {
	/* list of variadic arguments for sscanf */
	va_list args;
	va_start(args, format);
	
	struct conf *property = va_arg(args, struct conf *);
	struct conf *property_value = va_arg(args, struct conf *);
	
	va_end(args);

	check_expected_ptr(str);
	check_expected_ptr(format);

	bool success = mock_type(bool);

	if (success) {
		return __real___isoc99_sscanf(str, format, property, property_value);
	} else {
		return 1;
	}
}

static void test_getline_fail(bool test_getline_fail) {
	expect_not_value(__wrap_getline, lineptr, NULL);
	expect_not_value(__wrap_getline, n, NULL);
	expect_not_value(__wrap_getline, stream, NULL);

	if (test_getline_fail) {
		will_return(__wrap_getline, false);
	} else {
		will_return(__wrap_getline, true);
	}
}

static void test_sscanf_fail(bool test_sscanf_fail) {
	expect_not_value(__wrap___isoc99_sscanf, str, NULL);
	expect_string(__wrap___isoc99_sscanf, format, "%s %d");
	
	if (test_sscanf_fail) {
		will_return(__wrap___isoc99_sscanf, false);
	} else {
		will_return(__wrap___isoc99_sscanf, true);
	}
}

static void test_strcmp_fail(bool test_strcmp_fail) {
	expect_string(__wrap_strcmp, s1, config.property_from_file);
	expect_any(__wrap_strcmp, s2);
	
	if (test_strcmp_fail) {
		will_return(__wrap_strcmp, false);
	} else {
		will_return(__wrap_strcmp, true);
	}
}

static void test_fget_conf_value_success(void **state) {
	for (int i = 0; i < LINES_TO_TEST; i++) {
		test_getline_fail(false);
		test_sscanf_fail(false);
	}

	test_strcmp_fail(false);

	FILE *conf_file = open_config_file("ports.conf", strlen("ports.conf"));
	return assert_int_equal(fget_conf_value(conf_file, &config, "PORT:"), 8888);
}

static void test_fget_conf_value_fail(void **state) {
	for (int i = 0; i < LINES_TO_TEST; i++) {
		test_getline_fail(false);
		test_sscanf_fail(false);
	}

	test_strcmp_fail(false);
	test_getline_fail(false);
	test_sscanf_fail(false);
	test_getline_fail(false);

	FILE *conf_file = open_config_file("ports.conf", strlen("ports.conf"));
	return assert_int_equal(fget_conf_value(conf_file, &config, "pORT:"), -1);
}

int main() {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_fget_conf_value_success),
		cmocka_unit_test(test_fget_conf_value_fail)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}