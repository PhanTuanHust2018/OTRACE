//OAMH role: sever of OTRACE

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

void *socket_sever(void* port){
    printf("Thread socket sever OAMH run !\n");
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
            // ---------------socket connect and send request to L3_gNB-----------

        }
        close(connfd);
    }
    close(listenfd);    
    pthread_exit(NULL);

}
void *socket_client(void* port){
    int sockfd = -1;
    struct sockaddr_in sever_addr;
    char send_buffer[128];
    
    memset(send_buffer, 0, sizeof(send_buffer));
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    sever_addr.sin_family = AF_INET;
    sever_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sever_addr.sin_port = htons(*(int*)port);

    //bind(sockfd, (struct sockaddr_in*)&sever_addr,sizeof(sever_addr));

    if(connect(sockfd, (struct sockaddr*)&sever_addr,sizeof(sever_addr)) < 0){
        perror("connect fail\n");
    }
    else{
        struct RRC_OTRACE _RRC_OTRACE_START_REQ={MSG_START,MSG_REQ,1};
        memcpy(send_buffer,&_RRC_OTRACE_START_REQ,sizeof(send_buffer));
        if(send(sockfd, (void*)&send_buffer,sizeof(send_buffer),0) < 0){
            perror("send fail\n");
        }
    }
    close(sockfd);
    pthread_exit(NULL);
}
int main(){
    printf("OAMH process running ...\n");
    pthread_t t_OAMH_sever,t_OAMH_client;
    int *port_client,*port_sever;
    *port_client = 10001;
    *port_sever = 10000;
    pthread_create(&t_OAMH_sever,NULL,socket_sever,NULL);
    pthread_create(&t_OAMH_client,NULL,socket_client,(void*)port_client);
    pthread_join(t_OAMH_sever,NULL);
    pthread_join(t_OAMH_client,NULL);
    printf("OAMH process stop!\n");
    return 0;
}
