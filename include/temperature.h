#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <Adafruit_BME280.h>

class temperature
{
	public:
		temperature(void);

		bool is_active(void);

		float get_temperature(void);

	private:
		Adafruit_BME280 _bme;

		bool _active;
};

#endif  // TEMPERATURE_H
