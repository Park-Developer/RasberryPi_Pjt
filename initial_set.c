#include "initial_set.h"
int _TCP_SOCKET;

void init_set(){
    // GPIO Setting
    if(wiringPiSetup()==-1){
        printf("wiringpi error");
        exit(-1);
    }

    safety_setting();
    motor_setting();
    sensor_setting();
    //communication_setting();
}

void communication_setting(){
    // Communication Method
    // 1. Web Server - Main Controller => TCP/IP
    
    
	socklen_t clen; // 소켓 디스크립터 정의
	int n;
	struct sockaddr_in servaddr, cliaddr; // 구조체 정의
	char mesg[BUFSIZ];

	// 소켓 생성j
	if((_TCP_SOCKET=socket(AF_INET,SOCK_STREAM,0))<0){
		perror("socket()");
		//return -1;
	}
    printf("tCP socket(ini) %d" , _TCP_SOCKET);
    // 주소 구조체에 주소 지정
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=inet_addr("192.168.219.101");
	servaddr.sin_port=htons(TCP_PORT); // 사용할 포트 지정

    // bind 함수를 사용하여 서버 소켓의 주소 설정
	if(bind(_TCP_SOCKET,(struct sockaddr *)&servaddr,sizeof(servaddr))<0){
		perror("bind()");
		//return -1;	
	}
	
	// 동시에 접속하는 클라이언트의 처리를 위한 대기 큐를 설정
	if(listen(_TCP_SOCKET,8)<0){ // 최대 8개의 클라이언트가 동시 접속 대기할 수 있도록 설정
		perror("listen()");
		//return -1;
	} 

    clen=sizeof(cliaddr);
    char addr[100]; // TCP Client Address
    char sene_msg[100]="test message"; // send message
        
    int rn, csock = accept(_TCP_SOCKET,(struct sockaddr *)&cliaddr,&clen);

    // 네트워크 주소를 문자열로 변경
    inet_ntop(AF_INET,&cliaddr.sin_addr,addr,sizeof(addr));
    printf("Client is conneted : %s\n",addr);
	


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