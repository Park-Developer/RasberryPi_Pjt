#ifndef __SENSOR_H__
#define __SENSOR_H__
#include "initial_set.h"
#include "error.h"
/*
******************************
Variable Definition
******************************
*/

// 1. Ultrasonic Sensor
#define RETRY_CHECK_NUMBER 10

//bool encoder_sensing_state=false;

typedef struct Ultra_Data{
    int err_no;
    char err_msg[ERR_MSG_SIZE];
    double u1_dist;
    double u2_dist;

} Ultra_Data;

// 2. Encoder Sensor
#define ENCODER_DATA_SIZE 10000 

typedef struct Time_Data{
        long record_time;
        long accum_cnt;
} Time_Data;


typedef struct Encoder_Data{
    int err_no;
    char err_msg[ERR_MSG_SIZE];

    Time_Data enc1_record[ENCODER_DATA_SIZE];
    Time_Data enc2_record[ENCODER_DATA_SIZE];

} Encoder_Data;


/*
******************************
Function Definition
******************************
*/
void *encoder_on();
void *env_sensing();
void *ir_servo_sensing();
void make_Encoder_recordInfo(FILE * enc1_descp,FILE * enc2_descp);
void record_encoder_data(FILE * file_descp,double record_time,int enc_cnt); // record encoder data in txt file
char* measure_dist(int encoder_count); // measure distance using encoder count


Ultra_Data ultra_sensing();
Encoder_Data encoder_sensing();

#endif