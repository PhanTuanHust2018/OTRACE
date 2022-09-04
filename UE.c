#include <stdio.h>
#include <pthread.h>  //for thread 
#include <sys/socket.h> // for socket(), connect(), send(), and recv()
#include <stdlib.h>
#include <time.h>
#include <arpa/inet.h> //for sockaddr_in and inet_addr()
#include <string.h>
#include <unistd.h> //for close

#include "Param.h"



void *client_for_L3_gNB(){
    int sockfd = -1;
    struct sockaddr_in sever_addr;
    char buffer[1024];
    
    memset(buffer, 0, sizeof(buffer));
    
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    sever_addr.sin_family = AF_INET;
    sever_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sever_addr.sin_port = htons(10001);

    if(connect(sockfd, (struct sockaddr*)&sever_addr,sizeof(sever_addr)) < 0){
        perror("connect fail\n");
    }
    else{
        struct RRC_SETUP_REQUEST _rrc_set_up_request;
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
        _rrc_set_up_request.spare = 9;
        memset(buffer,0,sizeof(buffer));
        memcpy(buffer,&_rrc_set_up_request,sizeof(struct RRC_SETUP_REQUEST));
        printf("sizeof RRC_SETUP_REQUEST = %ld\n",sizeof(struct RRC_SETUP_REQUEST));
        // for(int i = 0; i < 48; i++){
        //     printf("%d\n",(uint8_t)*(buffer+i));
        // }
        if(send(sockfd,(void*)(&buffer),sizeof(buffer),0) < 0){
            perror("send RRC setup request fail\n");
        }
        while(1);
        close(sockfd);
        pthread_exit(0);
    }

}

int main(){
    printf("UE is attaching ...\n");
    pthread_t t_socket_client;
    pthread_create(&t_socket_client,NULL,client_for_L3_gNB, NULL);
    pthread_join(t_socket_client,NULL);
    printf("end UE\n");
    return 0;
}