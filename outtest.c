#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <softPwm.h>
#include <signal.h>
#include <sys/types.h>

/*
GPIO |wiringPi
22	 | 3
23	 | 4
24	 | 5
25	 | 6
12	 | 26	DC Motor PWM


*/


int main(int argc,char **argv){

    wiringPiSetup();
 
    pinMode(11,LOW);
 

    digitalWrite(11,HIGH); // gpio7 : I2C Switch

    //}
    return 0;
}

