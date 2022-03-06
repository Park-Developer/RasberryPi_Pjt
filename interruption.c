#include "interruption.h"

extern int _TCP_SOCKET; // TCP socket with Web server

void signal_setting(){
    signal(SIGINT, cmd_exit);
   
}
void cmd_exit(int sig){ 
    // : Ctrl+C(SIGINT)를 눌렀을 떄의 인터럽션
    
    printf("Prgram End by User");
    digitalWrite(RUN_SIGNAL,LOW);  // off the running signal 
    
    // Encoder Control
    //encoder_sensing_state=false; # 이거 살려야함
    
    // Motor Stop
    
        printf("this program will be exited in 3 seconds..\n");
        
        /*
        1. DC Motor Off
        */
        pinMode(3,OUTPUT);
        digitalWrite(3,LOW); // gpio22
        
        pinMode(4,OUTPUT);
        digitalWrite(4,LOW); // gpio23

        pinMode(5,OUTPUT);
        digitalWrite(5,LOW); // gpio24

        pinMode(6,OUTPUT);
        digitalWrite(6,LOW); // gpio25
        
        /*
        2. TCP socket clost 
        */
        close(_TCP_SOCKET); 
        
        exit(0);
} 