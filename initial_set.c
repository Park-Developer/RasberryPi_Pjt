#include "initial_set.h"

void init_set(){
    // GPIO Setting
    if(wiringPiSetup()==-1){
        printf("wiringpi error");
        exit(-1);
    }

    safety_setting();
    motor_setting();
    sensor_setting();
}

void safety_setting(){
    // Run Signal Setting
    pinMode(RUN_SIGNAL,OUTPUT);
    digitalWrite(RUN_SIGNAL,LOW);  // off the running signal 
}

void sensor_setting(){
    // Encoder Sensor Setting
    pinMode(ENCODER_1 , INPUT);
    pinMode(ENCODER_2 , INPUT);

    // Ultrasonic Sensor Setting
    pinMode (ULTRA_1_TRIG, OUTPUT);
    pinMode (ULTRA_1_ECHO, INPUT);
            
    pinMode (ULTRA_2_TRIG, OUTPUT);
    pinMode (ULTRA_2_ECHO, INPUT);
}

void motor_setting(){
    // Motor In Setting
    pinMode(MOTOR_L_IN1,OUTPUT);
    pinMode(MOTOR_L_IN2,OUTPUT);
    pinMode(MOTOR_R_IN1,OUTPUT);
    pinMode(MOTOR_R_IN2,OUTPUT);

    // PWM Setting
    pinMode(MOTOR_PWM,OUTPUT);

    digitalWrite(MOTOR_L_IN1,LOW); 
    digitalWrite(MOTOR_L_IN2,LOW);  
    digitalWrite(MOTOR_R_IN1,LOW); 
    digitalWrite(MOTOR_R_IN2,LOW); 
    
    softPwmCreate(MOTOR_PWM,0,100); 
}