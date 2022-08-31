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
    struct OTRACE_DATA_RRC_SETUP_REQ* _otrace_data = (struct OTRACE_DATA_RRC_SETUP_REQ*)malloc(sizeof(struct OTRACE_DATA_RRC_SETUP_REQ));
    // _otrace_data->data = (struct RRC_SETUP_REQUEST*)malloc(sizeof(struct RRC_SETUP_REQUEST));
    char buffer[1024];
    int _sockfd;
    _sockfd = *(int*)sockfd;
    if(recv(_sockfd,(void*)(&buffer),sizeof(buffer),0) < 0){
        perror("receive request fail\n");
    }
    else{
        char *index;
        index = buffer;
        _otrace_data->header.msg_type = (uint8_t)*index;
        _otrace_data->header.trace_type =  (uint8_t)*(index+1);
        _otrace_data->header.cell_id = (uint8_t)*(index+2);
        _otrace_data->header.subscriber_id = *((int*)(index)+1);
        _otrace_data->header.data_length = *((int*)(index)+2);
        _otrace_data->data.header.Subscription_ID = (uint8_t)*(index+16);
        _otrace_data->data.header.Misc_ID = (uint8_t)*(index+17);
        _otrace_data->data.header.Pkt_Version = (uint8_t)*(index+18);
        _otrace_data->data.header.RRC_release_Number_Major_minor = *((uint16_t*)(index)+10);
        _otrace_data->data.header.Radio_Bearer_ID = *((uint16_t*)(index)+11);
        _otrace_data->data.header.Physical_Cell_ID = *((uint16_t*)(index)+12);
        _otrace_data->data.header.NR_Cell_Global_ID = *((uint16_t*)(index)+13);
        _otrace_data->data.header.Freq = *((int*)(index)+7);
        _otrace_data->data.header.sfn = *((uint16_t*)(index)+16);
        _otrace_data->data.header.SubFrameNum = *((uint16_t*)(index)+17);
        _otrace_data->data.header.slot = *((uint16_t*)(index)+18);
        _otrace_data->data.header.PDU_Number = *((uint16_t*)(index)+19);
        _otrace_data->data.header.Msg_length = *((uint16_t*)(index)+20);
        _otrace_data->data.header.SIB_Mask_in_SI = *((uint16_t*)(index)+21);
        _otrace_data->data.ue_Identity_randomeVal = *((uint64_t*)(index)+6);
        _otrace_data->data.establishmentCause = *((uint8_t*)(index)+56);
        _otrace_data->data.spare = *((uint8_t*)(index)+57);
         
        printf("OTRACE HEADER\n\t{\n");
        printf("\tmsg_type: %d\n",_otrace_data->header.msg_type);
        printf("\ttrace_type: %d\n",_otrace_data->header.trace_type);
        printf("\tcell_id: %d\n",_otrace_data->header.cell_id);
        printf("\tsubscriber_id: %d\n",_otrace_data->header.subscriber_id);
        printf("\tdata_length: %d\n",_otrace_data->header.data_length);
        printf("\t}");
        printf("\n");
        printf("Subscription_ID = %d\n",_otrace_data->data.header.Subscription_ID);
        printf("Misc_ID = %d\n",_otrace_data->data.header.Misc_ID);
        printf("Pkt_Version = %d\n",_otrace_data->data.header.Pkt_Version);
        printf("RRC_release_Number_Major_minor = %d\n",_otrace_data->data.header.RRC_release_Number_Major_minor);
        printf("Radio_Bearer_ID = %d\n",_otrace_data->data.header.Radio_Bearer_ID);
        printf("Physical_Cell_I = %d\n",_otrace_data->data.header.Physical_Cell_ID);
        printf("NR_Cell_Global_ID = %d\n",_otrace_data->data.header.NR_Cell_Global_ID);
        printf("Freq = %d\n",_otrace_data->data.header.Freq);
        printf("sfn  = %d\n",_otrace_data->data.header.sfn);
        printf("SubFrameNum = %d\n",_otrace_data->data.header.SubFrameNum);
        printf("slot = %d\n",_otrace_data->data.header.slot);
        printf("PDU_Number = %d\n",_otrace_data->data.header.PDU_Number);
        printf("Msg_length = %d\n",_otrace_data->data.header.Msg_length);
        printf("SIB_Mask_in_SI = %d\n",_otrace_data->data.header.SIB_Mask_in_SI);
        printf("rrcSetupRequest\n");
        printf("{\n");
        printf("\tue_Identity_randomeVal:%ld\n",_otrace_data->data.ue_Identity_randomeVal);
        if(_otrace_data->data.establishmentCause == mo_Signalling)
            printf("\testablishmentCause: mo_Signalling\n");
        printf("\tspare: %d\n",_otrace_data->data.spare);
        printf("}\n");
        // free(_otrace_data.data);
        free(_otrace_data);

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
                printf("MSG_START_RESP\n");
            printf("Recive data resp_type: %d\n",_RRC_OTRACE_START_RESP.resp_type);
            if(_RRC_OTRACE_START_RESP.resp_type == SUCCESS)
                printf("SUCCEED\n");
            printf("start_time: %d\n",_RRC_OTRACE_START_RESP.start_time);
        /*--------------create new thread for reciving OTRACE data only---------*/
        if(_RRC_OTRACE_START_RESP.resp_type == SUCCESS){
            pthread_t t_trace_data;
            int *ptr_sockfd;
            ptr_sockfd = &sockfd;
            pthread_create(&t_trace_data,NULL,process_trace_data,(void*)ptr_sockfd);
            pthread_join(t_trace_data,NULL);
        }
        else{
            printf("RESQUET DATA NOT SUCCESS\n");
        }
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
    // pthread_create(&t_control,NULL,control_cmd, NULL);
    pthread_create(&t_socket_client,NULL,socket_client, NULL);
    // pthread_join(t_control,NULL);
    pthread_join(t_socket_client,NULL);
    printf("end OTRACE\n");
    return 0;
}
