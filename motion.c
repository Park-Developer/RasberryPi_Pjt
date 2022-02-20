#include "motion.h"

void *motor_control(void *arg)
{
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
return NULL;

}

 
