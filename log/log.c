#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>		// for date in log files
#include <stdlib.h>		// for malloc and free
#include <stdint.h>		// for error handling values

#include "conf.h"
#include "log.h"
#include "err_handlers.h"

#define LOG_PATH_LEN 32

static bool check_if_activated(FILE *log_conf_file, struct conf *config, const char *property) {
	fset_conf_state(log_conf_file, config, property);
	if (strcmp(config->property_state, "ON") == 0) {
		return true;
	}
	return false;
}

static int set_to_actual_date(char *date, size_t size, const char *format) {
	time_t now;
	struct tm result;
	struct tm *current_time;

	if (time(&now) < 0) {
		perror("Getting actual date and time");
		return ERROR_DATE_AND_TIME;
	}
	if ((current_time = localtime_r(&now, &result)) == NULL) {
		fprintf(stderr, "%s\n", "Actual date and time conversion failed");
		return ERROR_DATE_AND_TIME;
	}
	if (strftime(date, size, format, current_time) <= 0) {
		fprintf(stderr, "%s\n", "Actual date and time exceed buffer size bounds");
		return ERROR_DATE_AND_TIME;
	}
	return SUCCESS;
}

static void log_into_file(FILE *log_file, const char *type, const char *log_msg) {
	size_t max_size = 100;
	char *date_and_time = (char *)malloc(max_size);

	int8_t error_actual_date_and_time = set_to_actual_date(date_and_time, max_size, "%c");
	if (error_actual_date_and_time) {
		strcpy(date_and_time, "GETTING ACTUAL DATE AND TIME FAILED");
	}
	fprintf(log_file, "[ %s ] [ %s ] %s\n", date_and_time, type, log_msg);
	free(date_and_time);
}

static FILE *get_empty_log_file() {
	size_t max_size = 50;
	char *date = (char *)malloc(max_size);
	
	int8_t error_actual_date = set_to_actual_date(date, max_size, "%Y-%m-%d");
	if (error_actual_date) {
		fprintf(stderr, "%s\n", "Could not set filename to the current date");
		free(date);
		return NULL;
	}

	FILE *log_file = open_file(date, LOG_DIR, "a");
	if (log_file == NULL) {
		fprintf(stderr, "Could not create log file: [ %s ]\n", date);
		free(date);
		return NULL;
	}
	free(date);
	return log_file;
}

void push_log_into_file(struct conf *config, char *log_config_property, int log_property) {
	FILE *log_file = get_empty_log_file();
	FILE *log_conf_file = open_file("log.conf", CONF_DIR, "r");
	if (log_conf_file == NULL) {
		fprintf(stderr, "%s\n", "Could not open log file");
		return;
	}

	if (check_if_activated(log_conf_file, config, log_config_property)) {
		switch (log_property) {
			case STONESHIRE_STARTED: {
				log_into_file(log_file, "notice", "Stoneshire HTTP Server started correctly");
			} break;
			case LISTENING_SOCKET_INIT: {
				log_into_file(log_file, "info", "Listening socket has been created");
			} break;
		}
	}
	fclose(log_file);
	fclose(log_conf_file);
}