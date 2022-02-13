#include <stdio.h>
#include <math.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <stdbool.h>
#define SERVO 23
#include <pcf8591.h>
#include <pthread.h>
#define PCF 120

//GPIO Pin12 => Wiring Pi pin26
void *servo_motor_control(void *);
void *ir_read(void *);
pthread_mutex_t mid;

int main(){
    char sel;

    if(wiringPiSetup()==-1){
        return 1;
    }
    
    // Setup pcf8591 on base pin 120, and address 0x48
    pcf8591Setup (PCF, 0x48) ;

    softPwmCreate(SERVO,0,100); // softPwmCreate(1,0,200) says create a cycle 20 ms long made up of 200 steps (20 ms as 200 * 100 = 20000 microseconds).
    int AI_value0 ;
    int AI_value1;
    
    // Thread SEtting
    pthread_t servo_ctrl, ir_rd;
    pthread_mutex_init(&mid,NULL);

    pthread_create(&servo_ctrl,NULL,servo_motor_control,NULL);
    pthread_create(&ir_rd,NULL,ir_read,NULL);
  
    pthread_join(servo_ctrl,NULL);
    pthread_join(ir_rd,NULL);

    pthread_mutex_destroy(&mid);
    
    bool is_motorRun = false; // check if sevo motor is running


    while(1){
        // [0 Degree]
        softPwmWrite(SERVO,1);
        delay(2000);
        printf("0 deg :");
        AI_value0 = analogRead (PCF + 0) ;
        AI_value1 = analogRead (PCF + 1) ;
        printf("volt : %f , dist : %lf\n" , AI_value0*3.3/256,pow((AI_value0*3.3/256),-1.173)*29.988);
      

        // [90 Degree]
        softPwmWrite(SERVO,15); // neutral
        delay(2000);
        printf("90 deg :");
        AI_value0 = analogRead (PCF + 0) ;
        AI_value1 = analogRead (PCF + 1) ;
        printf("volt : %f , dist : %lf\n" , AI_value0*3.3/256,pow((AI_value0*3.3/256),-1.173)*29.988);

        // [180 Degree]
        softPwmWrite(SERVO,30);
        delay(2000);
        printf("180 deg :");
        AI_value0 = analogRead (PCF + 0) ;
        AI_value1 = analogRead (PCF + 1) ;
        printf("volt : %f , dist : %lf\n" , AI_value0*3.3/256,pow((AI_value0*3.3/256),-1.173)*29.988);
       
        // [90 Degree]
        softPwmWrite(SERVO,15); // neutral
        delay(2000);
        printf("90 deg :");
        AI_value0 = analogRead (PCF + 0) ;
        AI_value1 = analogRead (PCF + 1) ;
        printf("volt : %f , dist : %lf\n" , AI_value0*3.3/256,pow((AI_value0*3.3/256),-1.173)*29.988);
     
    }


    return 0;
}

void *servo_motor_control(void *arg)
{

}

void *ir_read(void *arg)
{

}