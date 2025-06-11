#ifndef AXIS_H
#define AXIS_H

#include <Adafruit_MPU6050.h>

class axis
{
	public:
		axis(void);

		bool is_active(void);

		void update(void);

		bool detected_step(void);
		bool detected_fall(void);

	private:
		Adafruit_MPU6050 _mpu;

		bool _active;

		sensors_event_t _accel;
};

#endif  // AXIS_H
