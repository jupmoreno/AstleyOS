#ifndef _LIB_TIME_H_
#define _LIB_TIME_H_

#include <define.h>
#include <systime.h>

int stime(time_st * time);
int gtime(time_st * time);

int vtime(time_st * time);
int lyear(int year);

#endif