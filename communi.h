#ifndef __COMMUNI_H__
#define __COMMUNI_H__
#include "initial_set.h"
#define SEND_SENSOR_NUMBER 20 // 보낼 수 있는 센서의 개수
#define SEND_MESSAGE_SIZE 300 // 송신 메세지의 크기

typedef struct _TCP_COMMU{
    int tcp_port; // tcp domain
    int cli_sock; // client socket(fd)
    int sensor_number;
    char send_msg[SEND_SENSOR_NUMBER][SEND_MESSAGE_SIZE] ;
    struct sockaddr_in* serv_addr;
    struct sockaddr_in* cli_addr;
    char cli_address[100];
} TCP_COMMU, *pTCP_COMMU;

pTCP_COMMU set_TCP();

#endif