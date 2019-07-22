#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "err_handlers.h"
#include "conf.h"

void fset_conf_state(FILE *conf_file, struct conf *config, const char *property) {
	char *line = NULL;
	size_t size_of_the_line = 0;

	while (getline(&line, &size_of_the_line, conf_file) != -1) {
		if (sscanf(line, "%49s %11s", config->property_from_file, config->property_state) == 2) {
			if ((strcmp(config->property_from_file, property)) == 0) {
				config->property_value = atoi(config->property_state);
				break;
			}
		} else if (ferror(conf_file)) {	
			print_error_and_exit("Configuration file read");
		}
	}

	free(line);
}

FILE *open_file(const char *filename, const char *parent_dir, const char *format) {
	char *file_path = (char *)malloc((strlen(parent_dir) + 1) + strlen(filename));
	if (file_path == NULL) {
		fprintf(stderr, "%s\n", "Malloc failed while opening configuration file");
		return NULL;
	}


	set_file_path(file_path, parent_dir, filename);

	FILE *f = fopen(file_path, format);
	free(file_path);
	return f;
}

int config_load_from_file(struct conf *config, const char *filename, const char *property) {
	FILE *config_file = open_file(filename, CONF_DIR, "r");
	if (config_file == NULL) {
		fprintf(stderr, "Opening config file: %s\n", filename);
		return ERROR_INVALID_CONFIG;
	}
	
	fset_conf_state(config_file, config, property);
	fclose(config_file);
	return SUCCESS;
}

void set_file_path(char *file_path, const char *parent_dir, const char *filename_with_ext) {
	strncpy(file_path, parent_dir, (strlen(parent_dir) + 1));
	strcat(file_path, filename_with_ext);
}