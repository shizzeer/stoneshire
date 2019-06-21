#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <netinet/in.h> // for sockaddr_in struct
#include <stdint.h> // for types in sockets structs
#include <arpa/inet.h> // for inet_ntop to test init_address_struct

#include "err_handlers.h"
#include "conf.h"
#include "log.h"

#define DEBUG

static void server_listening_init(int *listen_socket, struct conf *config, struct sockaddr_in *server_addr);
static bool port_is_valid(struct conf *config);

static int accept_the_client(int listen_socket, struct sockaddr_in *client_addr) {
	int addrlen = sizeof(*client_addr);
	int conn_socket = accept(listen_socket, (struct sockaddr *)client_addr, &addrlen);
	if (conn_socket < 0) {
		print_error_and_exit("Creating connection socket");
	}

	return conn_socket;
}

static void init_address_struct(struct sockaddr_in *server_addr, struct conf *config) {
	memset(server_addr, 0, sizeof(struct sockaddr_in));
	server_addr->sin_family = AF_INET;
	server_addr->sin_port = htons(config->property_value);
	server_addr->sin_addr.s_addr = INADDR_ANY;
}

int main() {

	int listen_socket, conn_socket;
	struct conf config;
	struct sockaddr_in server_addr, client_addr;

	server_listening_init(&listen_socket, &config, &server_addr);
	conn_socket = accept_the_client(listen_socket, &client_addr);

	return 0;
}

static bool port_is_valid(struct conf *config) {
	if (config->property_value >= 1024 && config->property_value <= 49151) {
		return true;
	} else {
		return false;
	}
}

static void server_listening_init(int *listen_socket, struct conf *config, struct sockaddr_in *server_addr) {
	
	*listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	if ((*listen_socket) < 0) {
		print_error_and_exit("Creating listening socket");
	}

	FILE *config_file = open_file("ports.conf", CONF_DIR, strlen("ports.conf"), strlen(CONF_DIR));
	if (config_file == NULL) {
		print_error_and_exit("Opening config file");
	}

	config->property_value = fget_conf_value(config_file, config, "PORT:");
	if (!port_is_valid(config)) {
		fprintf(stderr, "%s\n", "Listening port is not valid. Try to use port from 1024 to 49151 (user ports).");
		exit(EXIT_FAILURE);
	}

	init_address_struct(server_addr, config);

	#ifdef DEBUG
		int optval = 1;
		if (setsockopt(*listen_socket, SOL_SOCKET, (SO_REUSEADDR | SO_REUSEPORT), &optval, sizeof(optval)) < 0) {
			print_error_and_exit("Listening socket option");
		}
	#endif

	int binding_error = bind(*listen_socket, (struct sockaddr *)server_addr, sizeof(*server_addr));
	if (binding_error) {
		print_error_and_exit("Binding listening socket");
	}

	if (listen(*listen_socket, 5) < 0) {
		print_error_and_exit("Listen socket");
	}

	fclose(config_file);
}