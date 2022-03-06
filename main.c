#include "initial_set.h"
#include "communi.h"
#include "error.h"
#include "sensor.h"
#include "motion.h"
#include "interruption.h"


extern int _TCP_SOCKET; // TCP socket with Web server
pthread_mutex_t mid;

int main(int argc,char **argv){
    // Intial Setting
    init_set();
    printf("Project start!");
    interrupt_setting(); // Interruption Setting
 
    // TCP Communication Setting
    pTCP_COMMU tcp_info;
    tcp_info=set_TCP();
    printf("Connected Client Address : %s \n",tcp_info->cli_address);
    
    /*
    여기 있는애들 구조체의 함수로 전부 바꾸기 for tcp 통신에 대한 정보 전달을 위함
    */
    // Thread Setting
    pthread_t motor_ctrl, encoder_rd, env_sensors_rd, ir_servo_rd, velocity_ms;
    pthread_mutex_init(&mid,NULL);

    //pthread_create(&velocity_ms,NULL,measure_velocity,NULL);
    pthread_create(&motor_ctrl,NULL,motor_control,NULL);
    pthread_create(&encoder_rd,NULL,encoder_on,NULL);
    pthread_create(&env_sensors_rd,NULL,env_sensing,NULL);
    pthread_create(&ir_servo_rd,NULL,ir_servo_sensing,NULL);

    pthread_join(velocity_ms,NULL);
    pthread_join(motor_ctrl,NULL);
    pthread_join(encoder_rd,NULL);
    pthread_join(env_sensors_rd,NULL);
    pthread_join(ir_servo_rd,NULL);
    
    pthread_mutex_destroy(&mid);

    close(_TCP_SOCKET); 
    return 0;
}

