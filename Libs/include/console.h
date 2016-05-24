#ifndef _LIB_CONSOLE_H_
#define _LIB_CONSOLE_H_

#include <define.h>
#include <sysconsole.h>

void clear();

int sterminal(int index);

void tterminal(style_st color);
void bgterminal(style_st color);
void stterminal(style_st style);

#endif