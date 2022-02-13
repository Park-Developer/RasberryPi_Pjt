#ifndef __INTERRUPTION_H__
#define __INTERRUPTION_H__
#include "initial_set.h"
#include "sensor.h"
/*
I/O Setting
*/


void signal_setting();
void cmd_exit(int sig); // Ctrl+C(SIGINT)를 눌렀을 떄의 인터럽션
#endif