#include <stdio.h>
#include <math.h>
#include <wiringPi.h>
#include <softPwm.h>

#define SERVO 23
#include <pcf8591.h>

#define PCF 120

//GPIO Pin12 => Wiring Pi pin26

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
//gcc -o ultra ultra.c -lwiringPi