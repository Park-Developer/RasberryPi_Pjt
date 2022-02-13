#include "initial_set.h"
#include "error.h"
#include "sensor.h"
#include "motion.h"
#include "interruption.h"


pthread_mutex_t mid;

int main(int argc,char **argv){
    init_set();

    // Signal Setting
    signal_setting();
 
    digitalWrite(RUN_SIGNAL,HIGH); // Run signal ON 
    //encoder_sensing_state=true;

    // Thread Setting
    pthread_t motor_ctrl, encoder_rd, env_sensors_rd, ir_servo_rd;
    pthread_mutex_init(&mid,NULL);

    pthread_create(&motor_ctrl,NULL,motor_control,NULL);
    pthread_create(&encoder_rd,NULL,encoder_on,NULL);
    pthread_create(&env_sensors_rd,NULL,env_sensing,NULL);
    pthread_create(&ir_servo_rd,NULL,ir_servo_sensing,NULL);

    pthread_join(motor_ctrl,NULL);
    pthread_join(encoder_rd,NULL);
    pthread_join(env_sensors_rd,NULL);
    pthread_join(ir_servo_rd,NULL);
    
    pthread_mutex_destroy(&mid);

    return 0;
}

