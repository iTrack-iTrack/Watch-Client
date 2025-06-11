#include "microphone.h"

#include <Arduino.h>
#include <driver/adc.h>
#include <optional>

microphone *active_mic = nullptr;

void IRAM_ATTR on_timer(void)
{
	if (active_mic == nullptr || active_mic->is_sample_ready())
		return;

	int raw = adc1_get_raw(active_mic->get_pin());
	active_mic->set_sample(raw);
}

microphone::microphone(adc1_channel_t pin)
	: _pin{pin}, _timer{timerBegin(0, 80, true)}
{
	active_mic = this;

	adc1_config_width(ADC_WIDTH_BIT_12);
	adc1_config_channel_atten(_pin, ADC_ATTEN_DB_12);

	timerAttachInterrupt(_timer, &on_timer, false);
	timerAlarmWrite(_timer, 1000000 / 44100, true);
	timerAlarmEnable(_timer);
}

std::optional<int> microphone::consume_sample(void)
{
	if (!_sample_ready)
		return {};

	_sample_ready = false;
	return _sample;
}

adc1_channel_t microphone::get_pin(void)
{
	return _pin;
}

void microphone::set_sample(int sample)
{
	_sample_ready = true;
	_sample = sample;
}

bool microphone::is_sample_ready(void)
{
	return _sample_ready;
}
