#include <keyboard.h>
#include <keys.h>
#include <strings.h>

#define KEYBOARD_BUFFER_SIZE 256
#define BUFFER_NEXT(x) (((x) + 1) >= KEYBOARD_BUFFER_SIZE) ? 0 : ((x) + 1);

void keyboard_trigger(unsigned char scancode);

static int keyboard_current = _KEYBOARD_ENGLISH;

static key_st keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static int buffer_nextInsert = 0;
static int buffer_lastReturn = KEYBOARD_BUFFER_SIZE - 1;

static int key_pressed[KEYS_PRESSABLES] = {FALSE, FALSE, FALSE, FALSE};
static int key_on[KEYS_ONOFF] = {FALSE, TRUE, FALSE};

static int currykeys = 0;
static int waitingESC = FALSE;
static int enters = 0;

static key_st key_process(unsigned char scancode);

static int key_toggle(unsigned char scancode, int press);

static int key_ascii(unsigned char scancode);

/**
 * [Copied from its definition at: manager.c]
 * Recieves a key pointer and decides what action to perform, 
 * depending on the manager's flag status and the kind of key it is.
 *
 * IMPORTANT: This function should be only used by the keybardHandler function
 * at: keyboard.c. That's why its declaration is not included at: manager.h
 * @param key - the key to decide which action to perform
 */
extern void manage_key(key_st * key);

/**
 * Receives a scan code and process it, performing the necessary actions
 * to keep track of what has been done with the keyboard, and the key's buffer.
 *  
 * @param  scannedCode		the scan code to process
 * @returned             	OK if there was no problem while processing
 *                        	the scanned Code.
 *                        	COULD_NOT_PERFORM_OPERATION otherwise.
 */
void keyboard_trigger(unsigned char scancode) {
	key_st key;
	key = key_process(scancode);

	manage_key(&key);
}

int keyboard_set(int keyboard) {
	switch(keyboard) {
		case _KEYBOARD_SPANISH:
		case _KEYBOARD_ENGLISH:
			keyboard_current = keyboard;
			return OK;

		default:
			return _ERROR_KEYBOARD_INVALID;
	}
}

int keyboard_write(key_st * key) {
	int nextOfNextInsert = BUFFER_NEXT(buffer_nextInsert);
	currykeys ++;
	/*
	 * Asks this way because, when writing the expected '\n' key,
	 * there will be (buffer_nextInsert = nextOfNextInsert) == buffer_lastReturned.
	 * 
	 * If it was asking buffer_nextInsert == buffer_lastReturn, then it would be
	 * (buffer_nextInsert = nextOfNextInsert) == nextReturn, and getchar() would be
	 * returning KEYBOARD_BUFFER_EMPTY, and this is not the case.
	 */
	if(nextOfNextInsert == buffer_lastReturn) {
		if(key->value != '\n') {
			return _ERROR_KEYBOARD_BUFFER_FULL;	
		}
	}

	if(key->type != _KEY_TYPE_PRINTABLE) {
		return _ERROR_KEYBOARD_KEY_INVALID;
	}

	if(key->value == '\n') {
		enters++;
	}	

	keyboard_buffer[buffer_nextInsert] = *key;
	buffer_nextInsert = nextOfNextInsert;

	return OK;
}

int keyboard_canRead(void) {
	return enters > 0;
}

char keyboard_read(void) { // TODO: Unsigned?
	int character = 0;
	int nextReturn = BUFFER_NEXT(buffer_lastReturn);

	if(nextReturn == buffer_nextInsert) {
		return _ERROR_KEYBOARD_BUFFER_EMPTY;
	}

	character = keyboard_buffer[nextReturn].value;
	buffer_lastReturn = nextReturn;

	if(character == '\n') { 
		enters--;
	}
	currykeys --;
	return character;
}

int keyboard_delete(void) {
	/*
	 * Simulates the erasure of the previous key so as not to left the
	 * buffer inconsistent in case an interrupt changes the buffer state.
	 */
	int prevInsert = buffer_nextInsert - 1 == -1 ? KEYBOARD_BUFFER_SIZE - 1 : buffer_nextInsert - 1;

	if(prevInsert == buffer_lastReturn) {
		return _ERROR_KEYBOARD_OPERATION;
	}

	if(keyboard_buffer[prevInsert].value != '\n') {
		buffer_nextInsert = prevInsert;
		currykeys--;
		return OK;
	}

	return _ERROR_KEYBOARD_OPERATION;
}

static key_st key_process(unsigned char scancode) {
	key_st key = {0, 0};
	int press = 0;
	unsigned char c = scancode; // TODO: Porque a veces usa c y otras scancode?
	int ascii = 0;

	// Checks if it there is an inform of extended scan code
	if(scancode == SCANCODE_EXTENDED) {
		// If so, prepares the context to receive the extended scan code
		waitingESC = TRUE;
		key.type = _KEY_TYPE_EXTENDED;
		key.value = scancode; // TODO: key.value = 0?
		return key;
	}

	// Checks if it is a press or a release
	press = scancode <= SCANCODE_PRESS_MAX;

	if(!press) {
		c -= SCANCODE_PRESS_MAX;
	}

	// Clasifies the type of key, while does the coresponding actions.
	if(key_toggle(c, press)) {
		key.type = _KEY_TYPE_FUNCTIONAL;
		key.value = c;
	} else if(!press) {
		key.type = _KEY_TYPE_RELEASED;
		key.value = scancode;
	} else if((ascii = key_ascii(c)) != _KEY_NOTPRINTABLE) {
		key.type = _KEY_TYPE_PRINTABLE;
		key.value = ascii;
	} else {
		key.type = _KEY_TYPE_UNCLASSIFIED;
		key.value = c;
	}

	// Informs that, if the function was waiting an extended scan code to process, it has been processed
	if(waitingESC) {
		waitingESC = FALSE;
	}

	return key;
}

char canReadCurry(void){
	return currykeys;
}

static int key_toggle(unsigned char scancode, int press) {
	int pressed = TRUE;

	if(waitingESC) { // TODO: ??????
		switch(scancode) {
			case SCANCODE_ALTGR:
				key_pressed[ALTGR] = press;
				break;

			default:
				pressed = FALSE;
				break;
		}
	} else {
		switch(scancode) {
			case SCANCODE_CONTROL_LEFT:
				key_pressed[CONTROL] = press;
				break;

			case SCANCODE_SHIFT_LEFT:
			case SCANCODE_SHIFT_RIGTH:
				key_pressed[SHIFT] = press;
				break;

			case SCANCODE_ALT_LEFT:
				key_pressed[ALT] = press;
				break;

			case SCANCODE_CAPSLOCK:
				key_on[CAPSLOCK] = press != key_on[CAPSLOCK];
				break;

			case SCANCODE_NUMLOCK:
				key_on[NUMLOCK] = press != key_on[NUMLOCK];
				break;

			case SCANCODE_SCROLLLOCK:
				key_on[SCROLLLOCK] = press != key_on[SCROLLLOCK];
				break;

			// case SCANCODE_TAB: // TODO: ?????
			// 	pressed = press; // Released
			// 	break;

			default:
				pressed = FALSE;
				break;
		}
	}

	return pressed;
}


static int key_ascii(unsigned char scancode) {
	int ascii = 0, alternative = 0;

	if(key_pressed[ALTGR] || waitingESC) { // TODO: ?????
		return keys[keyboard_current][scancode][2];
	}

	if(KEY_ISNUMPAD(scancode)) {
		return keys[keyboard_current][scancode][key_on[NUMLOCK] ? 1 : 0];
	}

	/*	
	 * Done this way because, if the ascii code does not correspond to an 
	 * alphabetic character, the value from the requested key
	 * will be already saved at the variable ascii, taking into consideration
	 * the shiftPressed state.
	 */
	ascii = keys[keyboard_current][scancode][key_pressed[SHIFT] ? 1 : 0];

	// If ascii was an alphabetic character --> it gets the corresponding 
	// case of the key, based on the flags status.
	if(_CHAR_ISALPHA(ascii)) {
		alternative = key_pressed[SHIFT] != key_on[CAPSLOCK];
		ascii = keys[keyboard_current][scancode][alternative ? 1 : 0];
	}

	return ascii;
}
