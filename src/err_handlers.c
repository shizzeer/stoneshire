#include <stdio.h>
#include <stdlib.h>

void print_error_and_exit(const char *err_msg) {
	perror(err_msg);
	exit(EXIT_FAILURE);
}

int is_read_from_file_error(FILE *f, const char *err_info) {
	if (ferror(f)) {
		perror(err_info);
		return 1;
	} else {
		return 0;
	}
}

