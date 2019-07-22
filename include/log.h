#ifndef _LOG_H_
	#define _LOG_H_
	#define LOG_DIR "/home/shizzer/c/stoneshire/log/"
	#define ERROR_DATE_AND_TIME -1

	enum log_properties {STONESHIRE_STARTED, LISTENING_SOCKET_INIT};

	void push_log_into_file(struct conf *config, char *log_config_property, int log_property);
#endif