#include "communi.h"

pTCP_COMMU set_TCP(){
    // TCP SETIUP
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
	servaddr.sin_port=htons(TCP_PORT); // 사용할 포트 지정

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

        
    int rn, cli_socket = accept(tcp_socket,(struct sockaddr *)&cliaddr,&clen);

    // 네트워크 주소를 문자열로 변경
    inet_ntop(AF_INET,&cliaddr.sin_addr,addr,sizeof(addr));
    //printf("Client is conneted : %s\n",addr); 
    //  TCP SETUP
    
    // Setting return struct (pTCP_COMMU : pointer, TCP_COMMU : variable)
    pTCP_COMMU p=malloc(sizeof(TCP_COMMU));
    p->tcp_port=tcp_socket; // tcp domain
    p->cli_sock=cli_socket ; // client socket(fd)
    p->sensor_number=SENSOR_NUMBER;

    p->serv_addr=&servaddr;
    p->cli_addr=&cliaddr;
    strcpy(p->cli_address,addr);
    
    return p;

}

/*
    if ((rn=read(csock, mesg,sizeof(mesg)))<=0)	
	{
        perror("read()");
    }

    if(write(csock,tcp_sendData,sizeof(tcp_sendData))<=0){
	    perror("write()");
    }
*/