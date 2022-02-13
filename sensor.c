#include "sensor.h"


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
        /*
        // Ultra Sensor1 Echo HIGH Check
        if(digitalRead(ULTRA_1_ECHO)==HIGH && ultra1_start==false && ultra1_sensing==false){
            ultra1_start=true;
            U1_startTime = micros();
            printf("U1_startTime  %f",U1_startTime);
        }

        // Ultra Sensor2 Echo HIGH Check
        if(digitalRead(ULTRA_2_ECHO)==HIGH && ultra2_start==false && ultra2_sensing==false){
            ultra2_start=false;
            U2_startTime = micros();
             printf("U2_startTime  %f",U2_startTime);
        }

        // Ultra Sensor2 Echo LOW Check
        if(digitalRead(ULTRA_1_ECHO)==LOW && ultra1_start==true && ultra1_sensing==false){
            U1_travelTime = micros() - U1_startTime;
            U1_distance = U1_travelTime / 58;
            ultra1_sensing=true;

        }

        // Ultra Sensor2 Echo LOW Check
        if(digitalRead(ULTRA_2_ECHO)==LOW && ultra2_start==true && ultra2_sensing==false){
            U2_travelTime = micros() - U2_startTime;
            U2_distance = U2_travelTime / 58;
            ultra2_sensing=true;
        }
        
        // Normal Case
        if (ultra1_sensing==true && ultra2_sensing==true){
            result.err_no=0;
            strcpy(result.err_msg," ");
            result.u1_dist=U1_distance;
            result.u2_dist=U2_distance;

            break;
        }

        // Abnormal Case1
        if(ultra1_sensing==true && ultra2_sensing==false){
            u1_err_cnt++;
            
            if (u1_err_cnt>RETRY_CHECK_NUMBER){
                result.err_no=2;
                strcpy(result.err_msg,"ERROR_MSG_2"); // ori : strcpy(result.err_msg,ERROR_MSG_2);
                result.u1_dist=U1_distance;
                result.u2_dist=-1;

                break;
            }
        }

        // Abnormal Case2
        if(ultra1_sensing==false && ultra2_sensing==true){
            u2_err_cnt++;
            
            if (u2_err_cnt>RETRY_CHECK_NUMBER){
                result.err_no=1;
                strcpy(result.err_msg,"ERROR_MSG_1"); // ori : strcpy(result.err_msg,ERROR_MSG_1)
                result.u1_dist=-1;
                result.u2_dist=U2_distance;

                break;
            }
        }
        */  
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


    // count if state change
    while(1){ // ori: while(encoder_sensing_state){
      

        if(digitalRead(ENCODER_1) != past_enc1_state){
            printf("ENC1 %d\n",encoder1_cnt);
            encoder1_cnt= encoder1_cnt+1;
            if (encoder1_cnt>=ENCODER_DATA_SIZE){
                encoder1_cnt=0; // count reset
            
            }

            enc_data.enc1_record[encoder1_cnt].record_time=micros();
            enc_data.enc1_record[encoder1_cnt].accum_cnt=encoder1_cnt;
            
            past_enc1_state=digitalRead(ENCODER_1);
        }
        
        if(digitalRead(ENCODER_2) != past_enc2_state){
            printf("ENC2 %d\n",encoder2_cnt);
            encoder2_cnt= encoder2_cnt+1;
            if (encoder2_cnt>=ENCODER_DATA_SIZE){
                encoder2_cnt=0; // count reset
            
            }

            enc_data.enc2_record[encoder2_cnt].record_time=micros();
            enc_data.enc2_record[encoder2_cnt].accum_cnt=encoder2_cnt;
        
            past_enc2_state=digitalRead(ENCODER_2);
        }

    }
    
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