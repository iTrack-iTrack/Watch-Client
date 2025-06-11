#ifndef SD_CARD_H
#define SD_CARD_H

#include <optional>

class sd_card
{
	public:
		sd_card(int sclk_pin, int miso_pin, int mosi_pin, int cs_pin);

		bool is_active(void);

		void write_data(std::string file_name, std::string file_data);
		std::optional<std::string> read_data(std::string file_name);

	private:
		bool _active;
};

#endif  // SD_CARD_H
