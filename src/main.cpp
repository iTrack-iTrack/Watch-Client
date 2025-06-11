#include "axis.h"
#include "health_monitor.h"
#include "microphone.h"
#include "position.h"
#include "temperature.h"
#include "wifi_client.h"

#include <Arduino.h>
#include <ArduinoJson.h>

void setup(void)
{
	Serial.begin(115200);
	Serial.println("Hello, world!");
}

void loop(void)
{
	const std::string user_id = "1"; // TODO: Make this dynamic.

	static axis ag;
	static microphone mic(ADC1_CHANNEL_4); // GPIO-32
	static health_monitor hm;
	static position gps(16, 17);
	static temperature temp;
	static wifi_client wc;

	
	static unsigned long step = 0;
	static bool fall = false;
	if (ag.is_active())
	{
		ag.update();

		if (ag.detected_step())
			step += 1;

		if (!fall)
			fall = ag.detected_fall();
	}


	static std::optional<int> latest_valid_sample = {};
	std::optional<int> sample = mic.consume_sample();
	if (sample)
		latest_valid_sample = *sample;


	static std::optional<int> latest_valid_bpm = {};
	static std::optional<int> latest_valid_spo2 = {};
	static std::optional<bool> latest_placed_previous = {};
	static std::optional<bool> latest_placed_current = {};
	if (hm.is_active())
	{
		static bool placed_previous = false;

		int ir = hm.get_ir_value();
		int red = hm.get_red_value();

		bool placed_current = hm.placed_finger(ir);
		latest_placed_current = placed_current;
		if (placed_previous != placed_current)
		{
			latest_placed_previous = placed_previous;
			placed_previous = placed_current;
		}

		if (placed_current)
		{
			std::optional<int> bpm = hm.get_beats_per_minute(ir);
			if (bpm)
				latest_valid_bpm = bpm;

			// TODO: Implement Blood Oxygen function.
			std::optional<int> spo2 = hm.get_blood_oxygen(ir, red);
			if (spo2)
				latest_valid_spo2 = spo2;
		}
	}


	static std::optional<double> latest_valid_latitude = {};
	static std::optional<double> latest_valid_longitude = {};
	if (gps.is_active() && gps.has_refreshed())
	{
		latest_valid_latitude = gps.get_latitude();
		latest_valid_longitude = gps.get_longitude();
	}


	static std::optional<float> latest_valid_temperature = {};
	if (temp.is_active())
		latest_valid_temperature = temp.get_temperature();


	std::optional<bool> connected_wifi = wc.is_connected_to_wifi();
	std::optional<bool> connected_server = wc.is_connected_to_server();
	if (connected_wifi && !*connected_wifi)
		wc.connect_to_wifi("Odido-06F315", "7GMBN7BMRHV88W8C");
	else if (connected_server && !*connected_server)
		wc.connect_to_server("192.168.1.79", 8081);
		// wc.connect_to_server("itrackandi.watch", 8081); // TODO



	const long time_delay = 250;
	static long time_previous = millis();
	long time_current = millis();

	if (time_current - time_previous < time_delay)
		return;

	time_previous = time_current;



	JsonDocument json;
	json["user_id"] = user_id;


	if (ag.is_active())
	{
		Serial.printf(" | Step Made %d", step);
		json["step"] = step;
	}
	if (ag.is_active())
	{
		if (fall)
			Serial.printf(" | Fell!");
		json["fall"] = fall;
	}


	if (latest_valid_sample)
		Serial.printf(" | Sample %d", *latest_valid_sample);


	if (latest_valid_temperature)
	{
		Serial.printf(" | Temperature %f", *latest_valid_temperature);
		json["temperature"] = *latest_valid_temperature;
	}


	if (gps.is_active() && latest_valid_latitude)
	{
		Serial.printf(" | Latitude %f", *latest_valid_latitude);
		json["latitude"] = *latest_valid_latitude;
	}
	if (gps.is_active() && latest_valid_longitude)
	{
		Serial.printf(" | Longitude %f", *latest_valid_longitude);
		json["longitude"] = *latest_valid_longitude;
	}
	

	if (hm.is_active() && latest_valid_bpm)
	{
		Serial.printf(" | BPM %d", *latest_valid_bpm);

		if (latest_placed_current && *latest_placed_current)
			json["bpm"] = *latest_valid_bpm;
	}
	if (hm.is_active() && latest_valid_spo2)
	{
		Serial.printf(" | SPO2 %d", *latest_valid_spo2);

		if (latest_placed_current && *latest_placed_current)
			json["blood_oxygen"] = *latest_valid_spo2;
	}
	if (hm.is_active() && latest_placed_previous && latest_placed_current && *latest_placed_previous != *latest_placed_current)
	{
		Serial.printf(" | Finger %s Placed", *latest_placed_current ? "\b" : "Not");
		latest_placed_previous = {};
		latest_placed_current = {};
	}

	if (connected_wifi)
		Serial.printf(" | WiFi %s Connected", *connected_wifi ? "\b" : "Not");
	if (connected_wifi && connected_server)
		Serial.printf(" | Server %s Connected", *connected_server ? "\b" : "Not");
	if (connected_wifi && connected_server)
	{
		std::string str;
		serializeJson(json, str);

		wc.send_to_server(str);
		Serial.printf(" | Message Sent");
	}

	Serial.println();
}
