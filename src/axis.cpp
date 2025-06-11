#include "axis.h"

#include <Adafruit_MPU6050.h>
#include <Wire.h>

axis::axis(void)
{
	_active = _mpu.begin();
}

bool axis::is_active(void)
{
	return _active;
}

void axis::update(void)
{
	sensors_event_t temp_temperature;
	sensors_event_t temp_gyroscope;

	_mpu.getEvent(&_accel, &temp_gyroscope, &temp_temperature);
}

bool axis::detected_step(void)
{
	const float threshold = 12.0f;
	const unsigned long debounce = 300;

	static long time_previous = 0;
	long time_current = millis();

	float ax = _accel.acceleration.x;
	float ay = _accel.acceleration.y;
	float az = _accel.acceleration.z;
	float total = sqrt(ax*ax + ay*ay + az*az);

	bool is_step = total > threshold && (time_current - time_previous) > debounce;

	if (is_step)
		time_previous = time_current;

	return is_step;
}

bool axis::detected_fall(void)
{
	const float threshold = 4.0f;

	float ax = _accel.acceleration.x;
	float ay = _accel.acceleration.y;
	float az = _accel.acceleration.z;
	float total = sqrt(ax*ax + ay*ay + az*az);

	bool is_fall = total < threshold;
	return is_fall;
}
