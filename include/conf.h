#ifndef _CONF_H_
	#define _CONF_H_
	#define PROP_LEN 50

	struct conf {
		char property_from_file[PROP_LEN];
		int property_value;
		char conf_file_dir[35];	/* path to the configuration files */
	};

	FILE *open_config_file(const char *filename_with_ext, size_t filename_len, struct conf *config);
	int fget_conf_value(FILE *conf_file, struct conf *config, const char *property);
	void init_default_conf_dir(char *default_conf_dir, struct conf *config);
	void set_config_file_path(char *config_file_path, const char *filename_with_ext, struct conf *config);
#endif