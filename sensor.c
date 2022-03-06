#include "sensor.h"
extern int _TCP_SOCKET; // TCP socket with Web server
char *TCP_SENSOR_MSG;
int encoder1_cnt=0;
int encoder2_cnt=0;
double velocity_list[10]={0,};
int velocity_cnt=0;
double CUR_VELOCITY=0;
double TARGET_VELOCITY=12.55; // PWM 40 average speed
int PWM_VAL=40;

void *measure_velocity(){
    printf("debug : velocity measuring start");
    struct timeval  tv;
	double begin, cur_stamp,prior_stamp;
    prior_stamp=0;

    FILE *velocity_fp = fopen("Motor_velocity.txt", "w"); 
    
    //    fputs("[Encoder1 Data]\n", enc1_descp);

	gettimeofday(&tv, NULL);
	begin = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;

    double prior_dist=0;
    double cur_dist=0;
    double cur_velocity=0;

    bool feedback_on=true; // feedback 세팅
        int high_state=0;
        int low_state=0;
    while(1) { // 500ms 단위로 측정
        gettimeofday(&tv, NULL);
        cur_stamp = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
        
        char sample_time[30];
        char encoder_data[100];
        char current_velocity[100];

        // Sampling
        int calc_sample=0;
        double sample_begin_time=0;
        double sample_end_time=0;
        int sample_number=3;
        int sampling_count=0;



        if((cur_stamp-prior_stamp)>=500){ // 500ms 초과시
            
            sprintf(sample_time,"%f",(cur_stamp-begin));
            fputs(sample_time,velocity_fp); // 진행시간 기록
            fputs(", ",velocity_fp);     

            cur_dist=encoder1_cnt*(((double)WHEEL_LENGTH/ENCODER_PITCH_NUMBER));
            
            sprintf(encoder_data,"%f",cur_dist);
            fputs(encoder_data,velocity_fp); // 이동거리 기록
            fputs(", ",velocity_fp);
            
         
            //CUR_VELOCITY=(double)(cur_dist-prior_dist)*(1000)/(cur_stamp-prior_stamp);
            CUR_VELOCITY=(double)((cur_dist-prior_dist)*1000)/(cur_stamp-prior_stamp);
            printf(" [CUR_VELOCITY] %f ,%f ,%d\n",cur_stamp-prior_stamp, CUR_VELOCITY,PWM_VAL);
            //printf(" PWM_VAL %d \n", PWM_VAL);
            sprintf(current_velocity,"%f",CUR_VELOCITY);
            fputs(current_velocity,velocity_fp); // 속도 기록
            fputs("\n",velocity_fp);
            
            // FEEDBACK
            //int high_state=0;
            //int low_stat=0;

            if (feedback_on==true){               
                if(CUR_VELOCITY>TARGET_VELOCITY){
                    low_state=0;
                    high_state=high_state+1;
                    if(high_state==3){
                        //printf("high state!");
                        PWM_VAL=PWM_VAL-1;
                        softPwmWrite(MOTOR_PWM,PWM_VAL);
                        high_state=0;
                    }
                    
                }else if(CUR_VELOCITY<TARGET_VELOCITY){
                    high_state=0;
                    low_state=low_state+1;
                    if(low_state==3){
                           //printf("low state!");
                    PWM_VAL=PWM_VAL+1;
                    softPwmWrite(MOTOR_PWM,PWM_VAL);
                    low_state=0;
                    }
                }
            } 
            
            prior_stamp=cur_stamp;
            prior_dist=cur_dist;
            // FEEDBACK
        }


    }


}

void make_recordInfo(FILE * enc1_descp,FILE * enc2_descp){
    // [Common Property]
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char Record_year[20];
    char Record_month[20];
    char Record_day[20];
    char Record_hour[20];
    char Record_min[20];
    char Record_sec[20];
    
    sprintf(Record_year, "%d", tm.tm_year+1900);
    sprintf(Record_month, "%d", tm.tm_mon+1);
    sprintf(Record_day, "%d", tm.tm_mday);
    sprintf(Record_hour, "%d", tm.tm_hour);
    sprintf(Record_min, "%d", tm.tm_min);
    sprintf(Record_sec, "%d", tm.tm_sec);

    // [Encoder1 File Setting]
    fputs("[Encoder1 Data]\n", enc1_descp);
    fputs("-Record Date : ",enc1_descp);
    fputs(Record_year, enc1_descp);
    fputs(Record_month, enc1_descp);
    fputs(Record_day, enc1_descp);

    fputs("\n-Record Time : ",enc1_descp);
    fputs(Record_hour, enc1_descp);
    fputs(Record_min, enc1_descp);
    fputs(Record_sec, enc1_descp);

    fputs("\n-Col1 : time[sec]",enc1_descp);
    fputs("\n-Col2 : distance[cm]\n",enc1_descp);
    fputs("\n\n", enc1_descp);

    // [Encoder2 File Setting]
    fputs("[Encoder2 Data]\n", enc2_descp);
    fputs("-Record Date : ",enc2_descp);
    fputs(Record_year, enc2_descp);
    fputs(Record_month, enc2_descp);
    fputs(Record_day, enc2_descp);

    fputs("\n-Record Time : ",enc2_descp);
    fputs(Record_hour, enc2_descp);
    fputs(Record_min, enc2_descp);
    fputs(Record_sec, enc2_descp);

    fputs("\n-Col1 : time[sec]",enc2_descp);
    fputs("\n-Col2 : distance[cm]\n",enc2_descp);
    fputs("\n\n", enc2_descp);
}

char* measure_dist(int encoder_count){
    //double dist_per_pitch=(WHEEL_LENGTH/ENCODER_PITCH_NUMBER);
    static char measure_dist[DATA_RECORD_SIZE];

    sprintf(measure_dist, "%f",encoder_count*(((double)WHEEL_LENGTH/ENCODER_PITCH_NUMBER)));
    // recore unit : CM
    return measure_dist;
}

void record_encoder_data(FILE * file_descp,double record_time,int enc_cnt){
    char number_str[DATA_RECORD_SIZE];

    sprintf(number_str, "%f", record_time/1000000); // [sec] 단위로 저장(double -> String)
    strcat(number_str,",");
    strcat(number_str,measure_dist(enc_cnt));
    strcat(number_str,"\n");  

    fputs(number_str, file_descp);   
}

void *env_sensing(){
  
    while(1){
        ultra_sensing();
    }
}

void *encoder_on(){
    
    encoder_sensing();
}

Ultra_Data ultra_sensing(){
    // init set
    bool ultra1_sensing=false;
    bool ultra2_sensing=false;

    bool ultra1_start=false;
    bool ultra2_start=false;

    long U1_startTime=0;
    long U1_travelTime=0;
    int U1_distance=0;
    long U2_startTime=0;
    long U2_travelTime=0;
    int U2_distance=0;
    
    digitalWrite (ULTRA_1_TRIG, LOW);
    digitalWrite (ULTRA_2_TRIG, LOW);
    delayMicroseconds(2);

    digitalWrite (ULTRA_1_TRIG, HIGH);
    digitalWrite (ULTRA_2_TRIG, HIGH);
    delayMicroseconds(20);

    digitalWrite (ULTRA_1_TRIG, LOW);
    digitalWrite (ULTRA_2_TRIG, LOW);
    Ultra_Data result;

    int u1_err_cnt=0;
    int u2_err_cnt=0;

    while(1){
    
        long U1_startTime;
        long U1_travelTime;
        int U1_distance=0;
        long U2_startTime;
        long U2_travelTime;
        int U2_distance=0;
    

               //초음파 발생코드
        digitalWrite (ULTRA_1_TRIG, LOW);
        delayMicroseconds(2);
        digitalWrite (ULTRA_1_TRIG, HIGH);
        delayMicroseconds(20);
        digitalWrite (ULTRA_1_TRIG, LOW);
        
        //거리측정코드
        while(digitalRead(ULTRA_1_ECHO) == LOW);
        U1_startTime = micros();
        while(digitalRead(ULTRA_1_ECHO) == HIGH);
        U1_travelTime = micros() - U1_startTime;
        U1_distance = U1_travelTime / 58;
  
        //if(U1_distance < 400) printf( "Distance1: %dcm\n", U1_distance);
        delay(500);

        //초음파 발생코드
        digitalWrite (ULTRA_2_TRIG, LOW);
        delayMicroseconds(2);
        digitalWrite (ULTRA_2_TRIG, HIGH);
        delayMicroseconds(20);
        digitalWrite (ULTRA_2_TRIG, LOW);
        
        //거리측정코드
        while(digitalRead(ULTRA_2_ECHO) == LOW);
        U2_startTime = micros();
        while(digitalRead(ULTRA_2_ECHO) == HIGH);
        U2_travelTime = micros() - U2_startTime;
        U2_distance = U2_travelTime / 58;
  
        //if(U2_distance < 400) printf( "Distance2: %dcm\n", U2_distance);
        delay(500);   
    }

    //printf("Ultra1 Sensing : %d\n", U1_distance);
    //printf("Ultra2 Sensing : %d\n", U2_distance);
    return result;
}

Encoder_Data encoder_sensing(){
    int past_enc1_state=digitalRead(ENCODER_1);
    int past_enc2_state=digitalRead(ENCODER_2);


    Encoder_Data enc_data;

    // [File Record Setting]
    struct timeval  tv;
	double start_measure, enc1_stamp, enc2_stamp;
    //enc1_stamp : ENC1 경과 시간
    //enc2_stamp : ENC2 경과 시간

    gettimeofday(&tv, NULL);
	start_measure = micros(); // ori : (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
    FILE *enc1_fp = fopen(ENCODER1_DATA_FILE, "w");    
    FILE *enc2_fp = fopen(ENCODER2_DATA_FILE, "w");   

    // Data Record Info
    make_recordInfo(enc1_fp,enc2_fp); 
    
    // [Read & Record Encoder Value]
    double enc1_term=0;
    double enc1_cur_stamp=0;
    double enc1_past_stamp=0;
    double enc_sum=0;
    
    /* < 2022.03.07 01:24 origin Code >
    
    char tcp_sendData[100]="TCP end";
    printf("TCP SOCKET %d", _TCP_SOCKET);


    // TCP SETIUP
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
    //  TCP SETUP



    
    if ((rn=read(csock, mesg,sizeof(mesg)))<=0)	
	{perror("read()");
    }
	 	

	printf("Received Data : %s\n",mesg);

    */

    while(1){ // ori: while(encoder_sensing_state){
        /*
        ### ENCODER1 SENSING ###
        */
        if(digitalRead(ENCODER_1) != past_enc1_state && digitalRead(ENCODER_1)==1){
            enc1_cur_stamp=(micros()-start_measure);

            
            enc_sum=enc_sum+(enc1_cur_stamp-enc1_past_stamp);
            printf("ENC1 TERM %f \n",enc1_cur_stamp-enc1_past_stamp);
            
            /* TCP PART
            //snprintf(tcp_sendData,sizeof(tcp_sendData),"%f",enc_sum);
            printf("TCP SOCKET %d", _TCP_SOCKET);
            
            if(write(csock,tcp_sendData,sizeof(tcp_sendData))<=0){
			    perror("write()");
            }
       

             TCP PART*/

            enc1_past_stamp=enc1_cur_stamp;
            //printf("ENC1 %d\n",encoder1_cnt);
            encoder1_cnt= encoder1_cnt+1;
            if (encoder1_cnt>=13){
                //printf("ENC1 AVG %f \n",enc_sum/13);
                encoder1_cnt=0; // count reset
                enc_sum=0;
            }

            // Data Record
            enc_data.enc1_record[encoder1_cnt].record_time=micros()-start_measure;
            enc_data.enc1_record[encoder1_cnt].accum_cnt=encoder1_cnt;
            
            gettimeofday(&tv, NULL);
            enc1_stamp= enc_data.enc1_record[encoder1_cnt].record_time; //ori : (tv.tv_sec) * 1000 + ((tv.tv_usec) / 1000)-start_measure;
            
            record_encoder_data(enc1_fp,enc1_stamp,encoder1_cnt);

            past_enc1_state=digitalRead(ENCODER_1); // 이전 상태 저장
        }

        /*
        ### ENCODER2 SENSING ###
        */
        if(digitalRead(ENCODER_2) != past_enc2_state && digitalRead(ENCODER_2)==1){
            //printf("ENC2 %d\n",encoder2_cnt);
            encoder2_cnt= encoder2_cnt+1;
            if (encoder2_cnt>=ENCODER_DATA_SIZE){
                encoder2_cnt=0; // count reset
            
            }

            // Data Record
            enc_data.enc2_record[encoder2_cnt].record_time=micros()-start_measure;
            enc_data.enc2_record[encoder2_cnt].accum_cnt=encoder2_cnt;
        
            gettimeofday(&tv, NULL);
            enc2_stamp=enc_data.enc2_record[encoder2_cnt].record_time; //(tv.tv_sec) * 1000 + ((tv.tv_usec) / 1000)-start_measure;
            
            record_encoder_data(enc2_fp,enc2_stamp,encoder2_cnt);

            past_enc2_state=digitalRead(ENCODER_2); // 이전 상태 저장
        }
        
    }
    
    fclose(enc1_fp); 
    fclose(enc2_fp); 
    return enc_data;
}

void *ir_servo_sensing(){
    pcf8591Setup (PCF_ADDRESS, 0x48) ;
    softPwmCreate(SERVO_MOTOR,0,100); 
    int AI_value0 ;
    
    while(1){
        // [0 Degree]
        softPwmWrite(SERVO_MOTOR,1);
        delay(2000);
        //printf("0 deg :");
        AI_value0 = analogRead (PCF_ADDRESS + 0) ;
        //printf("volt : %f , dist : %lf\n" , AI_value0*3.3/256,pow((AI_value0*3.3/256),-1.173)*29.988);
      

        // [90 Degree]
        softPwmWrite(SERVO_MOTOR,15); // neutral
        delay(2000);
        //printf("90 deg :");
        AI_value0 = analogRead (PCF_ADDRESS + 0) ;
        //printf("volt : %f , dist : %lf\n" , AI_value0*3.3/256,pow((AI_value0*3.3/256),-1.173)*29.988);

        // [180 Degree]
        softPwmWrite(SERVO_MOTOR,30);
        delay(2000);
        //printf("180 deg :");
        AI_value0 = analogRead (PCF_ADDRESS + 0) ;
        //printf("volt : %f , dist : %lf\n" , AI_value0*3.3/256,pow((AI_value0*3.3/256),-1.173)*29.988);
       
        // [90 Degree]
        softPwmWrite(SERVO_MOTOR,15); // neutral
        delay(2000);
        //printf("90 deg :");
        AI_value0 = analogRead (PCF_ADDRESS + 0) ;
        //printf("volt : %f , dist : %lf\n" , AI_value0*3.3/256,pow((AI_value0*3.3/256),-1.173)*29.988);
     
    }
}

