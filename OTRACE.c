//OTRACE role: client of OAMH

#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <time.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

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


void *socket_client(){
    int sockfd = -1;
    struct sockaddr_in sever_addr;
    char send_buffer[128];
    
    memset(send_buffer, 0, sizeof(send_buffer));
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    sever_addr.sin_family = AF_INET;
    sever_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sever_addr.sin_port = htons(10000);

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
    pthread_create(&t_control,NULL,control_cmd, NULL);
    pthread_create(&t_socket_client,NULL,socket_client, NULL);
    pthread_join(t_control,NULL);
    pthread_join(t_socket_client,NULL);
    printf("end main");
    return 0;
}