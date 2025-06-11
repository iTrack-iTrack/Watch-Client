#include "position.h"

#include <HardwareSerial.h>
#include <TinyGPSPlus.h>

position::position(int pin_rx, int pin_tx)
	: _hs{2}
{
	_hs.begin(9600, SERIAL_8N1, pin_rx, pin_tx);
	_active = !!_hs;
}

bool position::is_active(void)
{
	return _active;
}

bool position::has_refreshed(void)
{
	while (_hs.available() > 0)
		_gps.encode(_hs.read());

	return _gps.location.isValid();
}

double position::get_latitude(void)
{
	return _gps.location.lat();
}

double position::get_longitude(void)
{
	return _gps.location.lng();
}
