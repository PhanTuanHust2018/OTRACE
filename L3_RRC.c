//L3_RRC role: sever of OTRACE

#include <stdio.h>
#include <pthread.h>  //for thread 
#include <sys/socket.h> // for socket(), connect(), send(), and recv()
#include <stdlib.h>
#include <time.h>
#include <arpa/inet.h> //for sockaddr_in and inet_addr()
#include <string.h>
#include <unistd.h> //for close

#include "Param.h"


void *socket_sever(void* port){
    printf("Thread socket sever L3_RRC run !\n");
    int listenfd = -1, connfd = -1;
    struct sockaddr_in server_addr;
    char send_buffer[1024];
    char recv_buffer[1024];
    time_t ticks;

    memset(send_buffer, 0, sizeof(send_buffer));
    memset(recv_buffer, 0, sizeof(recv_buffer));

    listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);

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
        /*------------recv request from OTRACE--------------- */
        if(recv(connfd,(void*)(&recv_buffer),sizeof(recv_buffer),0) < 0){
            perror("recive request fail\n");
        }
        else
        {
            struct RRC_OTRACE_START_REQ recv_cmd;
            memcpy((void*)&recv_cmd,(void*)recv_buffer,sizeof(recv_cmd));
            printf("Recive data msg_type:%d\n",recv_cmd.msg_type);
            if(recv_cmd.msg_type == MSG_START_REQ)
                printf("MSG_START_REQ\n");
            printf("Recive data trace_type: %d\n",recv_cmd.trace_type);
            if(recv_cmd.trace_type == TRACE_INTERFACE_Uu)
                printf("TRACE_INTERFACE_Uu\n");
            printf("Recive data cell_id: %d\n",recv_cmd.cell_id);
            printf("Recive data subscriber_id: %d\n",recv_cmd.subscriber_id);
            // ---------------send respond to OTRACE-----------
            if(recv_cmd.msg_type == MSG_START_REQ)
            {
                struct RRC_OTRACE_START_RESP _RRC_OTRACE_START_RESP = {
                    MSG_START_RESP,
                    SUCCEED,
                    100 // 100 second for trace, then shut off
                };
                memcpy(send_buffer,&_RRC_OTRACE_START_RESP,sizeof(_RRC_OTRACE_START_RESP));
                if(send(connfd,(void*)(&send_buffer),sizeof(send_buffer),0) < 0){
                    perror("send respond fail\n");
                }
            }

        }
        close(connfd);
    }
    close(listenfd);    
    pthread_exit(NULL);

}
// void *socket_client(void* port){
//     int sockfd = -1;
//     struct sockaddr_in sever_addr;
//     char send_buffer[128];
    
//     memset(send_buffer, 0, sizeof(send_buffer));
    
//     sockfd = socket(AF_INET, SOCK_STREAM, 0);
//     sever_addr.sin_family = AF_INET;
//     sever_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
//     sever_addr.sin_port = htons(*(int*)port);

//     //bind(sockfd, (struct sockaddr_in*)&sever_addr,sizeof(sever_addr));

//     if(connect(sockfd, (struct sockaddr*)&sever_addr,sizeof(sever_addr)) < 0){
//         perror("connect fail\n");
//     }
//     else{
//         struct RRC_OTRACE _RRC_OTRACE_START_REQ={MSG_START,MSG_REQ,1};
//         memcpy(send_buffer,&_RRC_OTRACE_START_REQ,sizeof(send_buffer));
//         if(send(sockfd, (void*)&send_buffer,sizeof(send_buffer),0) < 0){
//             perror("send fail\n");
//         }
//     }
//     close(sockfd);
//     pthread_exit(NULL);
// }
int main(){
    printf("L3_RRC process running ...\n");
    pthread_t t_L3_RRC_sever,t_L3_RRC_client;
    int *ptr_port_client,*ptr_port_sever;
    int _port_client = 10001;
    int _port_sever = 10000;
    ptr_port_client = &_port_client;
    ptr_port_sever = &_port_sever;
    pthread_create(&t_L3_RRC_sever,NULL,socket_sever,(void*)ptr_port_sever);
    // pthread_create(&t_L3_RRC_client,NULL,socket_client,(void*)port_client);
    pthread_join(t_L3_RRC_sever,NULL);
    // pthread_join(t_L3_RRC_client,NULL);
    printf("L3_RRC process stop!\n");
    return 0;
}
