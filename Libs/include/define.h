#ifndef _DEFINE_H_
#define _DEFINE_H_

#include <stdint.h>
#include <stddef.h>

#define _DEGUB_ENABLED

#define STRINGNIFY_MACRO(x) 		#x
#define STRINGNIFY_VALUE(x) 		STRINGNIFY_MACRO(x)
#define STRINGNIFY_BOOL(x) 			(x) ? "true" : "false"
#define STRINGNIFY_BOOL_YESNO(x) 	(x) ? "yes" : "no"

#define WORD_FIRST_HALF(x) 	(((x) & 0x0FF00) >> 8)
#define WORD_LAST_HALF(x) 	((x) & 0x0FF)

#ifndef NULL
#define NULL ((void *) 0)
#endif

#define TRUE 	1
#define FALSE 	0

#define OK 					0
#define ERROR 				-1
#define ERROR_OCCURRED(x) 	((x) < OK)

typedef uint8_t byte_t;
typedef uint16_t word_t;
typedef uint32_t dword_t;
typedef uint64_t qword_t;

typedef struct {
	char ** argv;
	unsigned int argc;
} args_t;

#endif