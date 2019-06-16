#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <stdbool.h>

#include "err_handlers.h"
#include "conf.h"

static void server_init(int *listen_socket, struct conf *config);
static bool port_is_valid(struct conf *config);

int main() {

	int listen_socket;
	struct conf config;

	server_init(&listen_socket, &config);

	return 0;
}

static bool port_is_valid(struct conf *config) {
	if (config->property_value >= 1024 && config->property_value <= 49151) {
		return true;
	} else {
		return false;
	}
}

static void server_init(int *listen_socket, struct conf *config) {
	*listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	if ((*listen_socket) < 0) {
		print_error_and_exit("Creating listening socket");
	}

	FILE *config_file = open_config_file("ports.conf", strlen("ports.conf"));
	if (config_file == NULL) {
		print_error_and_exit("Opening config file");
	}

	config->property_value = fget_conf_value(config_file, config, "PORT:");
	if (!port_is_valid(config)) {
		fprintf(stderr, "%s\n", "Listening port is not valid. Try to use port from 1024 to 49151 (user ports).");
		exit(EXIT_FAILURE);
	}

	/* -----------------------------------------------------------------------------------------------
		KOLEJNY KROK TO KONFIGURACJA POBRANEGO PORTU, A KONKRETNIE ZAMIANA PORTU Z WARTOSCI SHORT NA 
		WARTOSC "NETWORK BYTES" CZYLI TRZEBA ZAMIENIC WARTOSC Z DOMYSLNEGO LITTLE ENDIAN NA BIG
		ENDIAN DZIEKI FUNKCJI htons
		----------------------------------------------------------------------------------------------
	*/
	fclose(config_file);
}