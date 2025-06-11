#include "wifi_client.h"

#include <WiFi.h>

wifi_client::wifi_client(void)
{}

void wifi_client::connect_to_wifi(std::string wifi_ssid, std::string wifi_password)
{
	WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());
}

std::optional<bool> wifi_client::is_connected_to_wifi(void)
{
	const long delay = 10000;

	static long time_previous = millis();
	long time_current = millis();

	if (WiFi.status() != WL_CONNECTED)
	{
		if ((time_current - time_previous) <= delay)
			return {};

		time_previous = time_current;
		return false;
	}

	return true;
}

void wifi_client::connect_to_server(std::string server_url, int server_port)
{
	_client.stop();
	_client.connect(server_url.c_str(), server_port);
}

std::optional<bool> wifi_client::is_connected_to_server(void)
{
	const long delay = 10000;

	static long time_previous = millis();
	long time_current = millis();

	if (!_client.connected())
	{
		if ((time_current - time_previous) <= delay)
			return {};

		time_previous = time_current;
		return false;
	}

	return true;
}

void wifi_client::send_to_server(std::string message)
{
	_client.write(message.c_str());
	_client.flush();
}
