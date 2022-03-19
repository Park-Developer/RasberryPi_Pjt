#include "initial_set.h"
#include "communi.h"
#include "error.h"
#include "sensor.h"
#include "motion.h"
#include "interruption.h"


pthread_mutex_t mid;

extern int COMMAND_TCP_SOCKET;
extern int SENSING_TCP_SOCKET;
extern char* COMMAND;


int main(int argc,char **argv){
    // Intial Setting
    init_set();
    printf("Project start!");
    interrupt_setting(); // Interruption Setting
 
    // TCP Communication Setting
    //pTCP_COMMU tcp_info;
    //tcp_info=set_TCP();
    
    // Sensor Module Setting
    //pSENSOR_MODULE sensor_mod;
    //sensor_mod=set_sensor_module(tcp_info);
    
    // TCP SETTING /////////////////////////////////////

    
    int tcp_socket;
    char tcp_sendData[100]="TCP end";
    socklen_t clen; // 소켓 디스크립터 정의
	int n;
	struct sockaddr_in servaddr; // 구조체 정의
    struct sockaddr_in cliaddr; // 구조체 정의
	char mesg[BUFSIZ];

	// 소켓 생성
	if((tcp_socket=socket(AF_INET,SOCK_STREAM,0))<0){
		perror("socket()");
		//return -1;
	}


    // TCP Server 주소 지정
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=inet_addr("192.168.219.101");
	servaddr.sin_port=htons(TCP_PORT2); // 사용할 포트 지정

    // bind 함수를 사용하여 서버 소켓의 주소 설정
	if(bind(tcp_socket,(struct sockaddr *)&servaddr,sizeof(servaddr))<0){
		perror("bind()");
		//return -1;	
	}
	
	// 동시에 접속하는 클라이언트의 처리를 위한 대기 큐를 설정
	if(listen(tcp_socket,8)<0){ // 최대 8개의 클라이언트가 동시 접속 대기할 수 있도록 설정
		perror("listen()");
		//return -1;
	} 

    // TCP Client 설정
    clen=sizeof(cliaddr);
    char addr[100]; // TCP Client Address

    int rn;
    rn, SENSING_TCP_SOCKET = accept(tcp_socket,(struct sockaddr *)&cliaddr,&clen);

    // 네트워크 주소를 문자열로 변경
    inet_ntop(AF_INET,&cliaddr.sin_addr,addr,sizeof(addr));
    // TCP SETTING /////////////////////////////////////


    printf("Connected Client Address : %s \n",addr);





    /*
    여기 있는애들 구조체의 함수로 전부 바꾸기 for tcp 통신에 대한 정보 전달을 위함
    */
    // Thread Setting
    pthread_t tcp_commu, motor_ctrl, encoder_rd, env_sensors_rd, ir_servo_rd, velocity_ms;
    pthread_mutex_init(&mid,NULL);

    // [MOTION THREAD]
    
    pthread_create(&tcp_commu,NULL,real_time_tcp,NULL);
    pthread_create(&motor_ctrl,NULL,motor_control,NULL);
    // [SENSOR THREAD]
    pthread_create(&encoder_rd,NULL,encoder_on,NULL);
    printf("debug 생성전 ");
    pthread_create(&env_sensors_rd,NULL,env_sensing,NULL);
    
    //pthread_create(&ir_servo_rd,NULL,sensor_mod->ir_servo_AON,NULL);
 
    //printf("debug 생성 후 ");
    pthread_join(tcp_commu,NULL);
    pthread_join(motor_ctrl,NULL);
    pthread_join(encoder_rd,NULL);
    pthread_join(env_sensors_rd,NULL);
    //pthread_join(ir_servo_rd,NULL);
    
    pthread_mutex_destroy(&mid);

    close(SENSING_TCP_SOCKET);
    //free(tcp_info);
    //free(sensor_mod); 
    return 0;
}

