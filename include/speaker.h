#ifndef SPEAKER_H
#define SPEAKER_H

class speaker
{
	public:
		speaker(int bck_pin, int ws_pin, int data_pin);

		void output(void);

	private:
		const int _bck_pin;
		const int _ws_pin;
		const int _data_pin;
};

#endif  // SPEAKER_H
