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

/**
 * Determine if an integer is a power of 2
 * FROM: http://graphics.stanford.edu/~seander/bithacks.html#DetermineIfPowerOf2
 *
 * MORE INFO:
 * http://stackoverflow.com/questions/600293/how-to-check-if-a-number-is-a-power-of-2
 *
 * @param  x    number we want to see if v is a power of 2
 * @return      TRUE if power of 2; FALSE if not
 */
int numisPow2(unsigned int x) {
    return x && !(x & (x - 1)); // zero is not a power of two
}

/**
 * Round up to the next highest power of 2
 * FROM: http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
 *
 * It works by copying the highest set bit to all of the lower bits, and then adding one,
 * which results in carries that set all of the lower bits to 0 and one bit beyond
 * the highest set bit to 1. If the original number was a power of 2, then the decrement
 * will reduce it to one less, so that we round up to the same original value.
 *
 * MORE INFO:
 * http://stackoverflow.com/questions/466204/rounding-up-to-nearest-power-of-2
 * http://stackoverflow.com/questions/1322510/given-an-integer-how-do-i-find-the-next-largest-power-of-two-using-bit-twiddlin
 *
 * @param  x
 * @return      next highest power of 2 of 32-bit x
 */
int numnextPow2(unsigned int x) {
    if(numisPow2(x)) { // TODO: Neccesary?
        return x;
    }

    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x++;

    return x;
}
