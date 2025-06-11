#ifndef WIFI_CLIENT_H
#define WIFI_CLIENT_H

#include <WiFi.h>

class wifi_client
{
	public:
		wifi_client(void);

		void connect_to_wifi(std::string wifi_ssid, std::string wifi_password);
		std::optional<bool> is_connected_to_wifi(void);

		void connect_to_server(std::string server_url, int server_port);
		std::optional<bool> is_connected_to_server(void);

		void send_to_server(std::string message);

	private:
		WiFiClient _client;
};

#endif  // WIFI_CLIENT_H
