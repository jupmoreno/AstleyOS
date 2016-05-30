#include <define.h>
#include <pit.h>
#include <interrupts.h>
#include <log.h>

#define SECONDS_TO_PIT_CYCLES(x) (((x) * 1000) / time)

#define TIMERS 16

#define FREQUENCY_LOW 	18
#define FREQUENCY_HIGH 1193181

#define FREQUENCY_SLOWEST 0x10000
#define FREQUENCY_FASTEST 0x00001

#define SIGNAL_FREQUENCY (3579545 / 3)

#define TIME_DEFAULT 55

#define PORT_DATA_CH_1 0x40
#define PORT_DATA_CH_2 0x41
#define PORT_DATA_CH_3 0x42
#define PORT_CMD	   0x43

// PIT COMMANDS!
// http://wiki.osdev.org/Programmable_Interval_Timer
// Bits         Usage
//  6 and 7      Select channel :
//                  0 0 = Channel 0
//                  0 1 = Channel 1
//                  1 0 = Channel 2
//                  1 1 = Read-back command (8254 only)
//  4 and 5      Access mode :
//                  0 0 = Latch count value command
//                  0 1 = Access mode: lobyte only
//                  1 0 = Access mode: hibyte only
//                  1 1 = Access mode: lobyte/hibyte
//  1 to 3       Operating mode :
//                  0 0 0 = Mode 0 (interrupt on terminal count)
//                  0 0 1 = Mode 1 (hardware re-triggerable one-shot)
//                  0 1 0 = Mode 2 (rate generator)
//                  0 1 1 = Mode 3 (square wave generator)
//                  1 0 0 = Mode 4 (software triggered strobe)
//                  1 0 1 = Mode 5 (hardware triggered strobe)
//                  1 1 0 = Mode 2 (rate generator, same as 010b)
//                  1 1 1 = Mode 3 (square wave generator, same as 011b)
//  0            BCD/Binary mode: 0 = 16-bit binary, 1 = four-digit BCD
#define CMD_AC_LATCH 	0x00 // 00 00 0000
#define CMD_AC_LOW 		0x20 // 00 10 0000
#define CMD_AC_HIGH 	0x10 // 00 01 0000
#define CMD_AC_LOWHIGH 	0x30 // 00 11 0000

#define CMD_OP_RATE 	0x06 // 0000 011 0

#define CMD_MD_BIN 		0x00 // 0000 000 0
#define CMD_MD_BCD 		0x01 // 0000 000 1

typedef struct {
	unsigned int time;
} timer_t;

void pit_trigger(void);

static timer_t timers[TIMERS];
static int booleans[TIMERS];
static unsigned int time;

static int * pit_sleep_asign(double seconds);

extern void manage_time(void);

void pit_init(void) {
	int i;

	for(i = 0; i < TIMERS; i++) {
		timers[i].time = 0;
		booleans[i] = TRUE;
	}

	time = 55;
}

// void pit_set(unsigned int miliseconds) { // TODO:

// }

// uint32_t pit_get(void) { // TODO:
// 	return 1;
// }

void pit_trigger(void) {
	int i;

	for(i = 0; i < TIMERS; i++) {
		if(timers[i].time > 0) {
			timers[i].time--;
			if(timers[i].time == 0) {
				booleans[i] = FALSE;
			}
		}
	}

	manage_time();
}

void pit_wait(double seconds) {
	int * waiting;

	waiting = pit_sleep_asign(seconds);

	if(waiting == NULL) {
		return;
	}

	while(*waiting);
	*waiting = TRUE;
}

static int * pit_sleep_asign(double seconds) {
	int i;

	if(seconds <= 0) {
		return NULL;
	}

	for(i = 0; i < TIMERS; i++) {
		if(timers[i].time == 0) {
			timers[i].time = SECONDS_TO_PIT_CYCLES(seconds);
			#ifdef _DEGUB_ENABLED
			log("<PIT> Assigned timer: %d\n", i);
			#endif
			return &booleans[i];
		}
	}

	return NULL;
}
