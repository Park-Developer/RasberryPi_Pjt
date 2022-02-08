#include<stdio.h>
#include<wiringPi.h>
 
#define TRIG1 21 // trig GPIO 5
#define ECHO1 22 // echo GPIO 6

#define TRIG2 0 // trig GPIO 17
#define ECHO2 2 // echo GPIO 27

int main(void){
    long U1_startTime;
    long U1_travelTime;
    int U1_distance=0;
    long U2_startTime;
    long U2_travelTime;
    int U2_distance=0;
    if(wiringPiSetup()==-1) return 1;
        
    pinMode (TRIG1, OUTPUT);
    pinMode (ECHO1, INPUT);
            
    pinMode (TRIG2, OUTPUT);
    pinMode (ECHO2, INPUT);

    
    while(1){
        //초음파 발생코드
        digitalWrite (TRIG1, LOW);
        delayMicroseconds(2);
        digitalWrite (TRIG1, HIGH);
        delayMicroseconds(20);
        digitalWrite (TRIG1, LOW);
   
        
        //거리측정코드
        while(digitalRead(ECHO1) == LOW);
        U1_startTime = micros();
        while(digitalRead(ECHO1) == HIGH);
        U1_travelTime = micros() - U1_startTime;
        U1_distance = U1_travelTime / 58;
  
        if(U1_distance < 400) printf( "Distance1: %dcm\n", U1_distance);
        delay(500);

        //초음파 발생코드
        digitalWrite (TRIG2, LOW);
        delayMicroseconds(2);
        digitalWrite (TRIG2, HIGH);
        delayMicroseconds(20);
        digitalWrite (TRIG2, LOW);
   
        
        //거리측정코드
        while(digitalRead(ECHO2) == LOW);
        U2_startTime = micros();
        while(digitalRead(ECHO2) == HIGH);
        U2_travelTime = micros() - U2_startTime;
        U2_distance = U2_travelTime / 58;
  
        if(U2_distance < 400) printf( "Distance2: %dcm\n", U2_distance);
        delay(500);
    }

    return 0;
}