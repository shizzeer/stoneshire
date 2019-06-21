#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "err_handlers.h"
#include "conf.h"

#define VALUE_NOT_FOUND -1

int fget_conf_value(FILE *conf_file, struct conf *config, const char *property) {
	char *line = NULL;
	size_t size_of_the_line = 0;

	while (getline(&line, &size_of_the_line, conf_file) != -1) {
		if (sscanf(line, "%50s %d", config->property_from_file, &config->property_value) == 2) {
			if ((strcmp(config->property_from_file, property)) == 0) {
				free(line);
				return config->property_value;
			}
		} else if (ferror(conf_file)) {	
			print_error_and_exit("Configuration file read");
		}
	}

	free(line);
	return VALUE_NOT_FOUND;
}

FILE *open_file(const char *filename_with_ext, const char *parent_dir, size_t filename_len,
					   size_t parent_dir_len) {
	char *file_path = (char *)malloc((parent_dir_len + 1) + filename_len);
	if (file_path == NULL) {
		print_error_and_exit("Malloc for file_path in open_file");
	}


	set_file_path(file_path, parent_dir, filename_with_ext);

	FILE *f = fopen(file_path, "r");
	free(file_path);
	return f;
}

void set_file_path(char *config_file_path, const char *parent_dir, const char *filename_with_ext) {
	strncpy(config_file_path, parent_dir, (strlen(parent_dir) + 1));
	strcat(config_file_path, filename_with_ext);
}