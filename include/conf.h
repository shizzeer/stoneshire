#ifndef _CONF_H_
	#define _CONF_H_
	#define PROP_LEN 50
	#define MIN_INT_LEN 12
	#define CONF_DIR "/home/shizzer/c/stoneshire/conf/"
	#define ERROR_INVALID_CONFIG -2

	struct conf {
		char property_from_file[PROP_LEN];
		char property_state[MIN_INT_LEN];
		int property_value;
	};

	FILE *open_file(const char *filename, const char *parent_dir, const char *format);
	void fset_conf_state(FILE *config_file, struct conf *config, const char *property);
	int config_load_from_file(struct conf *config, const char *filename, const char *property);
	void set_file_path(char *file_path, const char *parent_dir, const char *filename_with_ext);
#endif