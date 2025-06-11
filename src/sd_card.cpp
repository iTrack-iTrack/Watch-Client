/*
#include "sd_card.h"

// TODO: Is this really working?
void loop_sd(void)
{
	static sd_card sd(18, 19, 23, 5);

	if (sd.is_active())
	{
		sd.write_data("/test", "hello, world!");

		std::optional<std::string> data = sd.read_data("/test");
		if (data)
			Serial.printf("%d ", *data);
	}
}
*/

#include "sd_card.h"

#include <SD.h>
#include <SPI.h>
#include <sstream>

sd_card::sd_card(int sclk_pin, int miso_pin, int mosi_pin, int cs_pin)
{
	SPI.begin(sclk_pin, miso_pin, mosi_pin, cs_pin);
	_active = SD.begin(cs_pin);
}

bool sd_card::is_active(void)
{
	return _active;
}

void sd_card::write_data(std::string file_name, std::string file_data)
{
	File write_file = SD.open(file_name.c_str(), FILE_WRITE);
	if (!write_file)
		return;

	write_file.println(file_data.c_str());
	write_file.close();
}

std::optional<std::string> sd_card::read_data(std::string file_name)
{
	File read_file = SD.open(file_name.c_str(), FILE_READ);
	if (!read_file)
		return {};

	std::stringstream ss;
	while (read_file.available())
		ss << (char) read_file.read();
	read_file.close();

	return ss.str();
}
