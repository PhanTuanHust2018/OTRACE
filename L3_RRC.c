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
    struct sockaddr_in server_addr,client_addr;
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
        socklen_t addr_size;
        addr_size = sizeof(struct sockaddr_in);
        // connfd = accept(listenfd,(struct sockaddr*)NULL, NULL);
        // connfd = accept(listenfd,(struct sockaddr*)&client_addr, (socklen_t*) sizeof(struct sockaddr_in));
        connfd = accept(listenfd,(struct sockaddr*)&client_addr, &addr_size);
        printf("client port: %d\n",ntohs(client_addr.sin_port));

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
            printf("Recive data duration: %d\n",recv_cmd.trace_duration);
            // ---------------send respond to OTRACE-----------
            if(recv_cmd.msg_type == MSG_START_REQ)
            {
                struct RRC_OTRACE_START_RESP _RRC_OTRACE_START_RESP = {
                    MSG_START_RESP,
                    SUCCESS, // FAIL or SUCCESS
                    100 // 100 second for trace, then shut off
                };
                memcpy(send_buffer,&_RRC_OTRACE_START_RESP,sizeof(_RRC_OTRACE_START_RESP));
                if(send(connfd,(void*)(&send_buffer),sizeof(send_buffer),0) < 0){
                    perror("send respond fail\n");
                }
                switch (recv_cmd.trace_type)
                {
                case TRACE_INTERFACE_Uu:
                {
                    struct RRC_SETUP_REQUEST _rrc_set_up_request; //= (struct RRC_SETUP_REQUEST*)malloc(sizeof(struct RRC_SETUP_REQUEST));
                    _rrc_set_up_request.header.Subscription_ID = sub_id;
                    _rrc_set_up_request.header.Misc_ID = misc_id;
                    _rrc_set_up_request.header.Pkt_Version = ptk_version;
                    _rrc_set_up_request.header.RRC_release_Number_Major_minor = RRC_release_number_major_minor;
                    _rrc_set_up_request.header.Radio_Bearer_ID = Radio_bearer_id;
                    _rrc_set_up_request.header.Physical_Cell_ID = PCI;
                    _rrc_set_up_request.header.NR_Cell_Global_ID = NR_cell_global_ID_;
                    _rrc_set_up_request.header.Freq = Freq_;
                    _rrc_set_up_request.header.sfn = SFN;
                    _rrc_set_up_request.header.SubFrameNum = SubFramNum;
                    _rrc_set_up_request.header.slot = SLOT;
                    _rrc_set_up_request.header.PDU_Number = PDU_number;
                    _rrc_set_up_request.header.Msg_length =Msg_length_;
                    _rrc_set_up_request.header.SIB_Mask_in_SI = SIB_Mask_in_SI_;

                    _rrc_set_up_request.ue_Identity_randomeVal = 8888; // need write a function for random UE ID
                    _rrc_set_up_request.establishmentCause = mo_Signalling;
                    _rrc_set_up_request.spare = 0;

                    struct OTRACE_DATA_RRC_SETUP_REQ*_otrace_data = (struct OTRACE_DATA_RRC_SETUP_REQ*)malloc(sizeof(struct OTRACE_DATA_RRC_SETUP_REQ));
                    _otrace_data->header.msg_type = MSG_DATA;
                    _otrace_data->header.trace_type = TRACE_INTERFACE_Uu;
                    _otrace_data->header.cell_id = 1;
                    _otrace_data->header.subscriber_id = 11111; // can xem lai
                    _otrace_data->header.data_length = sizeof(struct OTRACE_DATA_RRC_SETUP_REQ); // can xem lai
                    _otrace_data->data = _rrc_set_up_request;

                    memset(send_buffer,0,sizeof(send_buffer));
                    memcpy(send_buffer,_otrace_data,sizeof(struct OTRACE_DATA_RRC_SETUP_REQ));
                    if(send(connfd,(void*)(&send_buffer),sizeof(send_buffer),0) < 0){
                        perror("send respond fail\n");
                    }
                    // for (int i = 0; i< 64;i++){
                    //     printf("%d\n",(uint8_t)*(send_buffer+i));
                    // //     // printf("-------%ld\n",sizeof(struct RRC_SETUP_REQUEST));
                    // //     // printf("-------%ld\n",sizeof(struct RRC_SETUP_HEADER));
                    // }
                    free(_otrace_data);
                    // free(_rrc_set_up_request);
                    break;
                };
                case TRACE_INTERFACE_S1AP:
                    {
                        break;
                    }
                    
                case TRACE_INTERFACE_X2AP:
                    {
                        break;
                    }
                case TRACE_ALL:
                    {
                        break;
                    }
                default:
                    break;
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
