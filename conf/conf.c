#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "err_handlers.h"
#include "conf.h"

#define CONFIG_PATH_LEN 33
#define VALUE_NOT_FOUND -1

int fget_conf_value(FILE *conf_file, struct conf *config, const char *property) {
	char *line = NULL;
	size_t size_of_the_line = 0;

	while (getline(&line, &size_of_the_line, conf_file) != -1) {
		if (sscanf(line, "%s %d", config->property_from_file, &config->property_value) == 2) {
			if ((strcmp(config->property_from_file, property)) == 0) {
				puts(property);
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

FILE *open_config_file(const char *filename_with_ext, size_t filename_len) {
	char *config_file_path = (char *)malloc(CONFIG_PATH_LEN + filename_len);	
	if (config_file_path == NULL) {
		print_error_and_exit("Malloc for config_file_path");
	}


	set_config_file_path(config_file_path, filename_with_ext);

	FILE *config_file = fopen(config_file_path, "r");
	free(config_file_path);
	return config_file;
}

void set_config_file_path(char *config_file_path, const char *filename_with_ext) {
	strncpy(config_file_path, CONF_DIR, (strlen(CONF_DIR) + 1));
	strcat(config_file_path, filename_with_ext);
}