#include "initial_set.h"
int COMMAND_TCP_SOCKET;
int SENSING_TCP_SOCKET;
char* COMMAND;
void init_set(){
    // GPIO Setting
    printf("initial setting...");
    if(wiringPiSetup()==-1){
        printf("wiringpi error");
        exit(-1);
    }

    safety_setting();
    motor_setting();
    sensor_setting();
    signal_setting();
}

void signal_setting(){
    digitalWrite(RUN_SIGNAL,HIGH); // Run signal ON 
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
    
    softPwmCreate(MOTOR_PWM,0,1000);  
    /*
    int softPwmCreate (int pin, int initialValue, int pwmRange) ;
    
    The basic pulse unit is 100 micros long.
    The final paramater in softPwmCreate says how long each cycle is in basic pulse units.
    softPwmCreate(1,0,200) says create a cycle 20 ms long made up of 200 steps (20 ms as 200 * 100 = 20000 microseconds).
    softPwmWrite(1,185) say
    */
}