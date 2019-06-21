#ifndef _CONF_H_
	#define _CONF_H_
	#define PROP_LEN 50
	#define CONF_DIR "/home/shizzer/c/stoneshire/conf/"

	struct conf {
		char property_from_file[PROP_LEN];
		int property_value;
	};

	FILE *open_file(const char *filename_with_ext, const char *parent_dir, size_t filename_len, size_t parent_dir_len);
	int fget_conf_value(FILE *conf_file, struct conf *config, const char *property);
	void set_file_path(char *config_file_path, const char *parent_dir, const char *filename_with_ext);
#endif