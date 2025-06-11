#include "temperature.h"

#include <Adafruit_BME280.h>

temperature::temperature(void)
{
	_active = _bme.begin(0x76);
}

bool temperature::is_active(void)
{
	return _active;
}

float temperature::get_temperature(void)
{
	return _bme.readTemperature();
}
