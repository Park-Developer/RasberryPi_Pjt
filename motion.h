#ifndef __MOTION_H__
    #define __MOTION_H__
    #include "initial_set.h"
    void *motor_control(void *);
    
    void set_max_speed(); // 최대 속도로 설정
    void set_min_speed(); // 최소 속도로 설정
    void set_break(); // 동작 정지
    


#endif