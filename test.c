#include <wiringPi.h>
#include <stdio.h>
#include <sys/types.h>
#include <softPwm.h>
#include <signal.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

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



int main(){
      wiringPiSetup();
  softPwmCreate(MOTOR_PWM,0,100); 
    digitalWrite(MOTOR_L_IN1,LOW); 
    digitalWrite(MOTOR_L_IN2,HIGH);  
    digitalWrite(MOTOR_R_IN1,LOW); 
    digitalWrite(MOTOR_R_IN2,HIGH); 
    


    while(1){
        softPwmWrite(MOTOR_PWM,100);  
        delay(1000);
        
        softPwmWrite(MOTOR_PWM,50);
        delay(1000);
    }
return 0;
}
