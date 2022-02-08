#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <softPwm.h>
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>

/*
GPIO |wiringPi
22	 | 3
23	 | 4
24	 | 5
25	 | 6
12	 | 26	DC Motor PWM
*/

// Encoder Setting

#define ENCODER_1 25 // BCM_GPIO 26
#define ENCODER_2 27 // BCM_GPIO 16
#define ENCODER_CYCLE 21 

void interruptHandler(int sig){
        printf("this program will be exited in 3 seconds..\n");
        pinMode(3,OUTPUT);
        digitalWrite(3,LOW); // gpio22
        
        pinMode(4,OUTPUT);
        digitalWrite(4,LOW); // gpio23

        pinMode(5,OUTPUT);
        digitalWrite(5,LOW); // gpio24

        pinMode(6,OUTPUT);
        digitalWrite(6,LOW); // gpio25
        exit(0);
}

void *motor_control(void *);
void *encoder_read(void *);
pthread_mutex_t mid;

int main(int argc,char **argv){

    int gno;
    int encoder1_num=0;
    int encoder2_num=0;
    
    // Signal Setting
    signal(SIGINT, interruptHandler);

    // GPIO Setting
    wiringPiSetup();
 
    pinMode(26,OUTPUT);
    pinMode(ENCODER_1 , INPUT);
    pinMode(ENCODER_2 , INPUT);

    softPwmCreate(26,0,100); // gpio2

    //while(1){
    pinMode(3,OUTPUT);
    digitalWrite(3,LOW); // gpio22
    
    pinMode(4,OUTPUT);
    digitalWrite(4,LOW); // gpio23

    pinMode(5,OUTPUT);
    digitalWrite(5,HIGH); // gpio24

    pinMode(6,OUTPUT);
    digitalWrite(6,HIGH); // gpio25

    // Thread Setting
    pthread_t motor_ctrl, encoder_rd;
    pthread_mutex_init(&mid,NULL);

    pthread_create(&motor_ctrl,NULL,motor_control,NULL);
    pthread_create(&encoder_rd,NULL,encoder_read,NULL);
  
    pthread_join(motor_ctrl,NULL);
    pthread_join(encoder_rd,NULL);

    pthread_mutex_destroy(&mid);

    return 0;
}

void *motor_control(void *arg)
{
    while(TRUE){
        softPwmWrite(26,100);
        delay(1000);
        
        softPwmWrite(26,50);
        delay(1000);
    }
return NULL;

}

void *encoder_read(void *arg)
{
  // Encoder Read
  int encoder1_num=0;
  int encoder2_num=0;

  int past_enc1_state=digitalRead(ENCODER_1);
  int past_enc2_state=digitalRead(ENCODER_2);
  
  while(TRUE){
    if(digitalRead(ENCODER_1) != past_enc1_state){
      encoder1_num= encoder1_num+1;
      if (encoder1_num== ENCODER_CYCLE){
        encoder1_num=0;
      }
      printf("ENC1 %d\n",encoder1_num);
      
      past_enc1_state=digitalRead(ENCODER_1);
    }
    

    if(digitalRead(ENCODER_2) != past_enc2_state){
      encoder2_num= encoder2_num+1;
      if (encoder2_num== ENCODER_CYCLE){
        encoder2_num=0;
      }
      printf("ENC2 %d\n",encoder2_num);
      past_enc2_state=digitalRead(ENCODER_2);
    }
    
  }

  return NULL;
}