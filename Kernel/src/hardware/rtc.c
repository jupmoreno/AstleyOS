// Modified version of: http://wiki.osdev.org/CMOS#Writing_to_the_CMOS
#include <rtc.h>
#include <time.h>

#define SECONDS   0x00
#define MINUTES 0x02
#define HOURS 0x04
#define DAY 0x07
#define MONTH 0x08
#define YEAR 0x09
#define CENTURY 0x32

#define STATUS_REGISTER_A 0x0A
// Bit 7 of STATUS_REGISTER_A enabled
#define UPDATE_IN_PROGRESS_MASK 0x80

#define STATUS_REGISTER_B 0x0B
//Bit 2 of STATUS_REGISTER_B enabled
#define BINARY_MODE_MASK 0x04
//Bit 1 of STATUS_REGISTER_B enabled
#define HOUR_24_MODE_MASK 0x02

//Bit 7 of hour register enabled
#define HOUR_PM 0x80

#define BINARY_TO_BCD(x)	  ((((x) / 10) << 4) + ((x) % 10))
#define BCD_TO_BINARY(x)	 ((x) & 0x0F) + (((x) / 16) * 10)
#define HOUR_REMOVE_PM_INFORMATION(x)    ((x) & 0x7F)
#define HOUR_ADD_PM_INFORMATION(x,pm)     ((x) | (pm))
#define HOUR_TO_24_FORMAT(x)		  (((x)+12) % 24)
#define FULL_YEAR(x,y)			  ((x)+(y)*100)
 
uint8_t second;
uint8_t minute;
uint8_t hour;
uint8_t day;
uint8_t month;
uint16_t year;
uint8_t century;
uint8_t centuryChange = FALSE;

/**
 * Writes the given value at the desired CMOS address  
 * @param port  - the address to be written
 * @param value - the value to be written
 */
void _cmos_write(int port, int value);

/**
 * Reads from the CMOS address
 * @param  port - the address to be read  
 * @return	- the read value    
 */
int _cmos_read(int port);

/**
 * @return 1 if the RTC is currently updating. 0 otherwise.
 */
static int isUpdating(); 

int rtc_read(time_st * t) {
	uint8_t lastSecond;
	uint8_t lastMinute;
	uint8_t lastHour;
	uint8_t lastDay;
	uint8_t lastMonth;
	uint8_t lastYear;
	uint8_t lastCentury;
	uint8_t registerB;
	uint8_t binaryMode;
	uint8_t hours24Mode;
	uint8_t hourPM;

	// Note: This uses the "read registers until you get the same values
	// twice in a row" technique to avoid getting dodgy/inconsistent
	// values due to RTC updates
	while(isUpdating()); // Makes sure an update isn't in progress

	second = _cmos_read(SECONDS);
	minute = _cmos_read(MINUTES);
	hour = _cmos_read(HOURS);
	day = _cmos_read(DAY);
	month = _cmos_read(MONTH);
	year = _cmos_read(YEAR);
 
	do {
		lastSecond = second;
		lastMinute = minute;
		lastHour = hour;
		lastDay = day;
		lastMonth = month;
		lastYear = year;
		lastCentury = century;
 
		while (isUpdating()); // Makes sure an update isn't in progress

		second = _cmos_read(SECONDS);
		minute = _cmos_read(MINUTES);
		hour = _cmos_read(HOURS);
		day = _cmos_read(DAY);
		month = _cmos_read(MONTH);
		year = _cmos_read(YEAR);
		century = _cmos_read(CENTURY);
	} while(    (lastSecond != second) || (lastMinute != minute) 
			|| (lastHour != hour) || (lastDay != day) 
			|| (lastMonth != month) || (lastYear != year) 
			|| (lastCentury != century) );
 
	registerB = _cmos_read(STATUS_REGISTER_B);
	binaryMode = registerB & BINARY_MODE_MASK;

	hourPM = hour & HOUR_PM;
	// Converts BCD to binary values if necessary
	if (!binaryMode) { // bcdMode = TRUE
		second = BCD_TO_BINARY(second);
		minute = BCD_TO_BINARY(minute);
		hour = HOUR_REMOVE_PM_INFORMATION(hour);
		hour = BCD_TO_BINARY(hour);
		hour = HOUR_ADD_PM_INFORMATION(hour, hourPM);
		day = BCD_TO_BINARY(day);
		month = BCD_TO_BINARY(month);
		year = BCD_TO_BINARY(year);
		century = BCD_TO_BINARY(century);
	}
 
	// Converts 12 hour clock to 24 hour clock if necessary
	hours24Mode = registerB & HOUR_24_MODE_MASK;
	if (!(hours24Mode) && hourPM) {
		hour = HOUR_REMOVE_PM_INFORMATION(hour);
		hour = HOUR_TO_24_FORMAT(hour);
	}

	// Century adaptation: 
	// there virtual machines were the century updating works fine (qemu)
	// and some others where don't (VirtualBox).
	// This adaptation was made to work OK at VirtualBox.
	if (!centuryChange && year == 99) {
		centuryChange = TRUE;
	} 

	if (centuryChange && year == 00) {
		centuryChange = FALSE;
		century++;
		uint8_t aux = century;
		if (!binaryMode) { // bcdMode = TRUE 
			aux = BINARY_TO_BCD(aux);
		}
		_cmos_write(CENTURY, aux);
	}

	// Calculates the full (4-digit) year
	year = FULL_YEAR(year,century);

	// Loads the time structure
	t->second = second;
	t->minute = minute;
	t->hour = hour;
	t->day = day;
	t->month = month;
	t->year = year;

	return OK;
}


int rtc_write(time_st* t) {
	uint8_t registerB;
	uint8_t binaryMode;
	uint8_t hours24Mode;

	if(!vtime(t)) {
		return _RTC_ERROR_TIME_INVALID;
	}

	century = t->year/100;
	t->year -= (century*100);

	// Century adaptation: 
	// there virtual machines were the century updating works fine (qemu)
	// and some others where don't (VirtualBox).
	// This adaptation was made to work OK at VirtualBox.
	if (!centuryChange && t->year == 99) {
		centuryChange = TRUE;
	}
	else {
		centuryChange = FALSE;
	}

	registerB = _cmos_read(STATUS_REGISTER_B);
	hours24Mode = registerB & HOUR_24_MODE_MASK;
	if (!hours24Mode) {
		if (t->hour > 12) {
			t->hour -= 12;
		} 
		else if (t->hour == 0) {
			t->hour = 12;
		} 
	}

	binaryMode = registerB & BINARY_MODE_MASK;
	if (!binaryMode) { // bcdMode = TRUE
		second = BINARY_TO_BCD(t->second);
		minute = BINARY_TO_BCD(t->minute);
		hour = BINARY_TO_BCD(t->hour);
		day = BINARY_TO_BCD(t->day);
		month = BINARY_TO_BCD(t->month);
		year = BINARY_TO_BCD(t->year);
		century = BINARY_TO_BCD(century);
	} 
	else {
		second = t->second;
		minute = t->minute;
		hour = t->hour;
		day = t->day;
		month = t->month;
		year = t->year;
	}

	_cmos_write(SECONDS, second);
	_cmos_write(MINUTES, minute);
	_cmos_write(HOURS, hour);
	_cmos_write(DAY, day);
	_cmos_write(MONTH, month);
	_cmos_write(YEAR, year);
	_cmos_write(CENTURY, century);

	return OK;
}

static int isUpdating() {
	int statusRegisterA;

	statusRegisterA = _cmos_read(STATUS_REGISTER_A);

	return statusRegisterA & UPDATE_IN_PROGRESS_MASK;
}
