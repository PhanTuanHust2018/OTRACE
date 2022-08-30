//OTRACE role: client of L3_RRC

#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <time.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>

#include "Param.h"


char *rrc_otrace_datal;

void *process_trace_data(void* sockfd){
    char buffer[1024];
    int _sockfd;
    _sockfd = *(int*)sockfd;
    if(recv(_sockfd,(void*)(&buffer),sizeof(buffer),0) < 0){
        perror("receive request fail\n");
    }
    else{
        
    }

    printf("process data\n");

}

void *socket_client(){
    int sockfd = -1;
    struct sockaddr_in sever_addr;
    char send_buffer[1024];
    char recv_buffer[1024];
    
    memset(send_buffer, 0, sizeof(send_buffer));
    
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    sever_addr.sin_family = AF_INET;
    sever_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sever_addr.sin_port = htons(10000);

    //bind(sockfd, (struct sockaddr_in*)&sever_addr,sizeof(sever_addr));

    if(connect(sockfd, (struct sockaddr*)&sever_addr,sizeof(sever_addr)) < 0){
        perror("connect fail\n");
    }
    else{
        /*---------------send request------------*/
        struct RRC_OTRACE_START_REQ _RRC_OTRACE_START_REQ=
        {
            MSG_START_REQ,
            TRACE_INTERFACE_Uu,
            1, //cell_ID
            111111,  //Subsriber_ID
            100, //second
        };
        memcpy(send_buffer,&_RRC_OTRACE_START_REQ,sizeof(send_buffer));
        if(send(sockfd, (void*)&send_buffer,sizeof(send_buffer),0) < 0){
            perror("send fail\n");
        }
        /*--------------wait for respond-----------*/
        if(recv(sockfd,(void*)(&recv_buffer),sizeof(recv_buffer),0) < 0){
            perror("receive request fail\n");
        }
        else
        {
            struct RRC_OTRACE_START_RESP _RRC_OTRACE_START_RESP;
            memcpy((void*)&_RRC_OTRACE_START_RESP,(void*)recv_buffer,sizeof(recv_buffer));
            printf("Recive data msg_type:%d\n",_RRC_OTRACE_START_RESP.msg_type);
            if(_RRC_OTRACE_START_RESP.msg_type == MSG_START_RESP)
                printf("MSG_STOP_REQ\n");
            printf("Recive data resp_type: %d\n",_RRC_OTRACE_START_RESP.resp_type);
            if(_RRC_OTRACE_START_RESP.resp_type == SUCCESS)
                printf("SUCCEED\n");
            printf("start_time: %d\n",_RRC_OTRACE_START_RESP.start_time);
        }
        /*--------------create new thread for reciving OTRACE data only---------*/
        pthread_t t_trace_data;
        int *ptr_sockfd;
        ptr_sockfd = &sockfd;
        pthread_create(&t_trace_data,NULL,process_trace_data,(void*)&ptr_sockfd);
        pthread_join(t_trace_data,NULL);
    }
    close(sockfd);
    pthread_exit(NULL);

}
void *control_cmd(){
    while(1){
        printf("controller\n");
        sleep(1);
    }
    pthread_exit(NULL);
}

void *pull_trace_data(){
    while (1)
    {
        printf("trace data\n");
        sleep(1);
    }
    pthread_exit(NULL);
    
}
void user_interface(){
}

int main(){
    printf("OTRACE process running ...\n");
    pthread_t t_control, t_socket_client;
    // pthread_create(&t_control,NULL,control_cmd, NULL);
    pthread_create(&t_socket_client,NULL,socket_client, NULL);
    // pthread_join(t_control,NULL);
    pthread_join(t_socket_client,NULL);
    printf("end OTRACE\n");
    return 0;
}
