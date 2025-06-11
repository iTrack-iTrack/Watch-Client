#ifndef MICROPHONE_H
#define MICROPHONE_H

#include <Arduino.h>
#include <driver/adc.h>
#include <optional>

class microphone
{
	public:
		microphone(adc1_channel_t pin);

		std::optional<int> consume_sample(void);

		adc1_channel_t get_pin(void);

		void set_sample(int sample);
		bool is_sample_ready(void);

	private:
		hw_timer_t *_timer = NULL;

		adc1_channel_t _pin;

		volatile int  _sample = 0;
		volatile bool _sample_ready = false;
};

#endif  // MICROPHONE_H
