#ifndef _ERR_HANDLERS_H_
	#define _ERR_HANDLERS_H_
	#define SUCCESS 0

	int is_socket_error(int *, const char *);
	void print_error_and_exit(const char *err_msg);
#endif	