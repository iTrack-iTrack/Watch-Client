#include "health_monitor.h"

#include <heartRate.h>
#include <MAX30105.h>
#include <optional>
#include <spo2_algorithm.h>
#include <Wire.h>

health_monitor::health_monitor(void)
{
	if (!_max.begin(Wire, I2C_SPEED_STANDARD))
	{
		_active = false;
		return;
	}

	_max.setup();
	_max.setPulseAmplitudeIR(0x0A);
	_max.setPulseAmplitudeRed(0x0A);
	_max.setPulseAmplitudeGreen(0);

	_active = true;
}

bool health_monitor::is_active(void)
{
	return _active;
}

int health_monitor::get_ir_value(void)
{
	return _max.getIR();
}

int health_monitor::get_red_value(void)
{
	return _max.getRed();
}

bool health_monitor::placed_finger(int ir)
{
	const int finger_threshold = 30000;
	return ir >= finger_threshold;
}

std::optional<int> health_monitor::get_beats_per_minute(int ir)
{
	const byte rate_size = 4;
	static byte rates[rate_size];
	static byte rate_spot = 0;

	static int previous_beat = millis();

	int has_beat = checkForBeat(ir);

	if (!has_beat)
		return {};

	int current_beat = millis();
	float delta = current_beat - previous_beat;
	previous_beat = current_beat;

	float bpm = 60.0f / (delta / 1000.0f);
	if (bpm <= 20 || bpm >= 200)
		return {};

	rates[rate_spot] = (byte) bpm;
	rate_spot = (rate_spot + 1) % rate_size;

	int sum = 0;
	for (byte i = 0; i < rate_size; i++)
		sum += rates[i];

	return sum / rate_size;
}

std::optional<int> health_monitor::get_blood_oxygen(int ir, int red)
{ return {}; }
