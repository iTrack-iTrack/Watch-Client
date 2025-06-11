#ifndef POSITION_H
#define POSITION_H

#include <HardwareSerial.h>
#include <TinyGPSPlus.h>

class position
{
	public:
		position(int pin_rx, int pin_tx);

		bool is_active(void);

		bool has_refreshed(void);

		double get_latitude(void);
		double get_longitude(void);

	private:
		bool _active;

		HardwareSerial _hs;
		TinyGPSPlus _gps;
};

#endif  // POSITION_H
