#include <numbers.h>
#include <define.h>

#define CHAR_IS_INT(x) (((x) >= '0') && ((x) <= '9'))
#define CHAR_TO_INT(x) ((x) - '0')

int numstr(const char * string) {
    int value = 0;
    int negative = FALSE;

    if (*string == '-') {
        negative = TRUE;
        ++string;
    }

    while(CHAR_IS_INT(*string)) {
        value = (value * 10) + CHAR_TO_INT(*string);
        ++string;
    }

    return negative ? -value : value;
}

int numstrl(const char * string, int * value) {
    int digits = 0;
    int negative = FALSE;

    if (*string == '-') {
        negative = TRUE;
        ++string;
    }

    while(CHAR_IS_INT(*string)) {
        *value = (*value * 10) + CHAR_TO_INT(*string);
        ++string;
        digits++;
    }

    if(negative) {
    	*value = -*value;
    }

    return digits;
}
