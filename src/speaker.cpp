/*
#include "speaker.h"

// TODO: Need speaker for testing...
void test_spkr(void)
{
	speaker spkr(26, 25, 27);

	// TODO: Maybe make it more dynamic?
	for (;;)
		spkr.output();
}
*/

#include "speaker.h"

#include <driver/i2s.h>
#include <math.h>

#define SAMPLE_RATE       44100
#define AMPLITUDE         10
#define FREQUENCY         440
#define SAMPLES_PER_CYCLE SAMPLE_RATE / FREQUENCY

int16_t test_sine_wave[SAMPLES_PER_CYCLE * 2];

speaker::speaker(int bck_pin, int ws_pin, int data_pin)
	: _bck_pin{bck_pin}, _ws_pin{ws_pin}, _data_pin{data_pin}
{
	i2s_config_t i2s_config =
	{
		.mode                 = (i2s_mode_t) (I2S_MODE_MASTER | I2S_MODE_TX),
		.sample_rate          = SAMPLE_RATE,
		.bits_per_sample      = I2S_BITS_PER_SAMPLE_16BIT,
		.channel_format       = I2S_CHANNEL_FMT_RIGHT_LEFT,
		.communication_format = I2S_COMM_FORMAT_STAND_I2S,
		.intr_alloc_flags     = ESP_INTR_FLAG_LEVEL1,
		.dma_buf_count        = 8,
		.dma_buf_len          = 64,
		.use_apll             = false
	};

	i2s_pin_config_t pin_config =
	{
		.bck_io_num   = _bck_pin,
		.ws_io_num    = _ws_pin,
		.data_out_num = _data_pin,
		.data_in_num  = I2S_PIN_NO_CHANGE
	};

	i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
	i2s_set_pin(I2S_NUM_0, &pin_config);

	for (int i = 0; i < SAMPLES_PER_CYCLE; i++)
	{
		test_sine_wave[i * 2]     = (int16_t) (AMPLITUDE * sin((2.0 * M_PI * i) / SAMPLES_PER_CYCLE));
		test_sine_wave[i * 2 + 1] = test_sine_wave[i * 2];
	}
}

void speaker::output(void)
{
	size_t bytes_written;
	i2s_write(I2S_NUM_0, test_sine_wave, sizeof test_sine_wave, &bytes_written, portMAX_DELAY);
}
