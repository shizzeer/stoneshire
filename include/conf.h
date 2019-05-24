#ifndef _CONF_H_
	#define _CONF_H_
	#define PROP_LEN 50

	struct conf {
		char property_from_file[PROP_LEN];
		int property_value;
	};

	FILE *open_config_file(const char *filename_with_ext, size_t filename_len);
	int fget_conf_value(FILE *conf_file, struct conf *config, const char *property);
#endif