#include "motion.h"
extern double CUR_VELOCITY;
extern double TARGET_VELOCITY;

void *motor_control(void *arg)
{
    digitalWrite(MOTOR_L_IN1,LOW); 
    digitalWrite(MOTOR_L_IN2,HIGH);  
    digitalWrite(MOTOR_R_IN1,LOW); 
    digitalWrite(MOTOR_R_IN2,HIGH); 
    


    //PRWM40 average velocity : 12.55cm/s
    
    softPwmWrite(MOTOR_PWM,500);  // Intial Velocity Setting

    // Velocity Feedback with average
    


 
return NULL;

}

void set_max_speed(){

}

void set_min_speed(){

}

