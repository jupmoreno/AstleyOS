#include <sound.h>
#include <ports.h>
#include <pit.h>

typedef struct {
	unsigned int frequency;
	int time;
} beep_st;

static void sound_play(unsigned int frequency);
static void sound_stop(void);

void sound_beep(unsigned int frequency, double time) {
	sound_play(frequency);
	pit_wait(time); // TODO: pit_later(time)
	sound_stop();
}

static void sound_play(unsigned int frequency) {
	uint32_t divv;
	uint8_t tmp;

	if(frequency == 0) {
		return;
	}

	//Set the PIT to the desired frequency
	divv = 1193180 / frequency;
	_port_write_byte(0x43, 0xb6);
	_port_write_byte(0x42, (uint8_t) (divv));
	_port_write_byte(0x42, (uint8_t) (divv >> 8));

	//And play the sound using the PC speaker
	tmp = _port_read_byte(0x61);
 	if (tmp != (tmp | 3)) {
		_port_write_byte(0x61, tmp | 3);
	}
}

static void sound_stop(void) {
	uint8_t tmp;

	tmp = _port_read_byte(0x61) & 0xFC;
	_port_write_byte(0x61, tmp);
}
