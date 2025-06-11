#ifndef HEALTH_MONITOR_H
#define HEALTH_MONITOR_H

#include <MAX30105.h>
#include <optional>

class health_monitor
{
	public:
		health_monitor(void);

		bool is_active(void);

		int get_ir_value(void);
		int get_red_value(void);

		bool placed_finger(int ir);

		std::optional<int> get_beats_per_minute(int ir);
		std::optional<int> get_blood_oxygen(int ir, int red);

	private:
		bool _active;

		MAX30105 _max;
};

#endif  // HEALTH_MONITOR_H
