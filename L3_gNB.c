#include <stdio.h>
#include <pthread.h>  //for thread 
#include <sys/socket.h> // for socket(), connect(), send(), and recv()
#include <stdlib.h>
#include <time.h>
#include <arpa/inet.h> //for sockaddr_in and inet_addr()
#include <string.h>
#include <unistd.h> //for close

#define MSG_START 1
#define MSG_STOP 2
#define MSG_REQ 0
#define MSG_RES 1

struct RRC_OTRACE
{
    int msg_type;
    int msg_direction;
    int msg_id;
};


void *sever_for_OAMH(){

}

void *sever_for_UE(){

}

void *socket_sever(void* port){
    printf("Thread socket sever L3_gNB run !\n");
    int listenfd = -1, connfd = -1;
    struct sockaddr_in server_addr;
    char send_buffer[128];
    char recv_buffer[512];
    time_t ticks;

    memset(send_buffer, 0, sizeof(send_buffer));
    memset(recv_buffer, 0, sizeof(recv_buffer));

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    if(listenfd < 0){
        perror("error on creating socket");
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(*(int*)port);

    bind(listenfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    listen(listenfd,10);

    while (1)
    {
        connfd = accept(listenfd,(struct sockaddr*)NULL, NULL);
        /*------------recv request--------------- */
        if(recv(connfd,(void*)(&recv_buffer),sizeof(recv_buffer),0) < 0){
            perror("recive data fail\n");
        }
        else
        {
            struct RRC_OTRACE recv_cmd;
            memcpy((void*)&recv_cmd,(void*)recv_buffer,sizeof(recv_cmd));
            printf("Recive data msg_id: %d\n",recv_cmd.msg_id);
            printf("Recive data msg_type: %d\n",recv_cmd.msg_type);
            printf("Recive data msg_direction: %d\n",recv_cmd.msg_direction);
        }
        close(connfd);
    }
    close(listenfd);    
    pthread_exit(NULL);

}

int main(){
    printf("L3_gNB is running ...");
    pthread_t t_socket_sever_UE, t_socket_sever_OAMH;
    int *port_UE, *port_OAMH;
    *port_UE = 10002;
    *port_OAMH = 10001;
    printf("Port UE %d \n",*port_UE);
    printf("Port OAMH %d \n",*port_OAMH);

    pthread_create(&t_socket_sever_UE,NULL,socket_sever,(void*)port_UE); 
    pthread_create(&t_socket_sever_OAMH,NULL,socket_sever,(void*)port_OAMH); 
    pthread_join(t_socket_sever_UE,NULL);
    pthread_join(t_socket_sever_OAMH,NULL);
    return 0;
}