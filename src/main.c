#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "err_handlers.h"
#include "conf.h"

static void server_init(int *listen_socket, struct conf *config);

int main() {

	int listen_socket;
	struct conf config;

	server_init(&listen_socket, &config);

	return 0;
}

static void server_init(int *listen_socket, struct conf *config) {
	*listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	if ((*listen_socket) < 0) {
		print_error_and_exit("Creating listening socket");
	}

	init_default_conf_dir("/home/shizzer/stoneshire/conf/", config);

	FILE *config_file = open_config_file("ports.conf", strlen("ports.conf"), config);
	if (config_file == NULL) {
		print_error_and_exit("Opening config file");
	}

	config->property_value = fget_conf_value(config_file, config, "PORT:");

	/* -----------------------------------------------------------------------------------------------
		KOLEJNY KROK TO KONFIGURACJA POBRANEGO PORTU, A KONKRETNIE ZAMIANA PORTU Z WARTOSCI SHORT NA 
		WARTOSC "NETWORK BYTES" CZYLI TRZEBA ZAMIENIC WARTOSC Z DOMYSLNEGO LITTLE ENDIAN NA BIG
		ENDIAN DZIEKI FUNKCJI htons
		----------------------------------------------------------------------------------------------
	*/
	fclose(config_file);
}