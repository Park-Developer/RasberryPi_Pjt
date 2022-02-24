#include "sensor.h"

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
  
        if(U1_distance < 400) printf( "Distance1: %dcm\n", U1_distance);
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
  
        if(U2_distance < 400) printf( "Distance2: %dcm\n", U2_distance);
        delay(500);   
    }

    printf("Ultra1 Sensing : %d\n", U1_distance);
    printf("Ultra2 Sensing : %d\n", U2_distance);
    return result;
}

Encoder_Data encoder_sensing(){
    int past_enc1_state=digitalRead(ENCODER_1);
    int past_enc2_state=digitalRead(ENCODER_2);
    int encoder1_cnt=0;
    int encoder2_cnt=0;

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
    while(1){ // ori: while(encoder_sensing_state){
      

        if(digitalRead(ENCODER_1) != past_enc1_state && digitalRead(ENCODER_1)==1){
            printf("ENC1 %d\n",encoder1_cnt);
            encoder1_cnt= encoder1_cnt+1;
            if (encoder1_cnt>=ENCODER_DATA_SIZE){
                encoder1_cnt=0; // count reset
            
            }

            // Data Record
            enc_data.enc1_record[encoder1_cnt].record_time=micros()-start_measure;
            enc_data.enc1_record[encoder1_cnt].accum_cnt=encoder1_cnt;
            
            gettimeofday(&tv, NULL);
            enc1_stamp= enc_data.enc1_record[encoder1_cnt].record_time; //ori : (tv.tv_sec) * 1000 + ((tv.tv_usec) / 1000)-start_measure;
            
            record_encoder_data(enc1_fp,enc1_stamp,encoder1_cnt);

            past_enc1_state=digitalRead(ENCODER_1); // 이전 상태 저장
        }
        
        if(digitalRead(ENCODER_2) != past_enc2_state && digitalRead(ENCODER_2)==1){
            printf("ENC2 %d\n",encoder2_cnt);
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
        printf("0 deg :");
        AI_value0 = analogRead (PCF_ADDRESS + 0) ;
        printf("volt : %f , dist : %lf\n" , AI_value0*3.3/256,pow((AI_value0*3.3/256),-1.173)*29.988);
      

        // [90 Degree]
        softPwmWrite(SERVO_MOTOR,15); // neutral
        delay(2000);
        printf("90 deg :");
        AI_value0 = analogRead (PCF_ADDRESS + 0) ;
        printf("volt : %f , dist : %lf\n" , AI_value0*3.3/256,pow((AI_value0*3.3/256),-1.173)*29.988);

        // [180 Degree]
        softPwmWrite(SERVO_MOTOR,30);
        delay(2000);
        printf("180 deg :");
        AI_value0 = analogRead (PCF_ADDRESS + 0) ;
        printf("volt : %f , dist : %lf\n" , AI_value0*3.3/256,pow((AI_value0*3.3/256),-1.173)*29.988);
       
        // [90 Degree]
        softPwmWrite(SERVO_MOTOR,15); // neutral
        delay(2000);
        printf("90 deg :");
        AI_value0 = analogRead (PCF_ADDRESS + 0) ;
        printf("volt : %f , dist : %lf\n" , AI_value0*3.3/256,pow((AI_value0*3.3/256),-1.173)*29.988);
     
    }
}

