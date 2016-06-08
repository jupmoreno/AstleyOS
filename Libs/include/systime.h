#ifndef _SYSTIME_H_
#define _SYSTIME_H_

#include <define.h>

#define _RTC_ERROR_TIME_INVALID -1
#define _SYSCALL_ERROR_TIME_OPERATION_INVALID -2

typedef struct {
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
	uint8_t month;
	uint16_t year;
} time_st;

enum time_ops {
	_TIME_OPERATION_SET = 0,
	_TIME_OPERATION_GET
};

#endif
