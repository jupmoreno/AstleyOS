#include <time.h>

extern int systime(int operation, time_st * time);

int stime(time_st * time) {
	return systime(_TIME_OPERATION_SET, time);
}

int gtime(time_st * time) {
	return systime(_TIME_OPERATION_GET, time);
}

int vtime(time_st * time) {
	static int days[][13]= {
		{0,31,28,31,30,31,30,31,31,30,31,30,31},
		{0,31,29,31,30,31,30,31,31,30,31,30,31}
	};
	int leap, maxDays;

	if(time->second >= 60 || time->minute >= 60 || time-> hour >= 24) {
		return FALSE;
	}

	if(time->day >=32 || time->day == 0|| time->month >= 13 || time->month == 0) {
		return FALSE;
	}

	if(time->year < 1980 || time->year > 2250) {
		return FALSE;
	}

	leap = lyear(time->year);
	maxDays = days[leap][time->month];
	
	if(time->day > maxDays) {
		return FALSE;
	}
	
	return TRUE;
}

int lyear(int year) {
	if(year % 4 == 0 && (year % 400 == 0 || year % 100 != 0)) {
		return TRUE;
	}

	return FALSE;
}
