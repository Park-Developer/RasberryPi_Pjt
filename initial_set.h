#ifndef __INITIAL_SET_H__
#define __INITIAL_SET_H__
#include <wiringPi.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <softPwm.h>
#include <signal.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <pcf8591.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// I/O Setting
#define RUN_SIGNAL 12 // BCM_GPIO 10

#define ENCODER_1 25 // BCM_GPIO 26
#define ENCODER_2 27 // BCM_GPIO 16
#define ENCODER_CYCLE 21 

#define MOTOR_L_IN1 3 // BCM_GPIO 22
#define MOTOR_L_IN2 4 // BCM_GPIO 23
#define MOTOR_R_IN1 5 // BCM_GPIO 24
#define MOTOR_R_IN2 6 // BCM_GPIO 25

#define MOTOR_PWM 26  // BCM_GPIO 12

#define SERVO_MOTOR 23

#define ULTRA_1_TRIG 21 // BCM_GPIO 5
#define ULTRA_1_ECHO 22 // BCM_GPIO 6
#define ULTRA_2_TRIG 0  // BCM_GPIO 17
#define ULTRA_2_ECHO 2  // BCM_GPIO 27
 
#define ERR_MSG_SIZE 30 

#define PCF_ADDRESS 120

// Mobility Setting
#define WHEEL_LENGTH 22 // Unit : cm
#define ENCODER_PITCH_NUMBER 27

#define ENCODER1_DATA_FILE "enc1_data.txt"
#define ENCODER2_DATA_FILE "enc2_data.txt"
#define DATA_RECORD_SIZE 150

/* [SENSOR SETTING] */
#define SENSOR_NUMBER 7
void init_set();
void motor_setting();
void sensor_setting();
void safety_setting();
void communication_setting();
void signal_setting();

/* [Communication Setting]*/
#define TCP_PORT 5101
#define TCP_PORT2 5102

#endif