// L3_RRC role: sever of OTRACE

#include <stdio.h>
#include <pthread.h>    //for thread
#include <sys/socket.h> // for socket(), connect(), send(), and recv()
#include <stdlib.h>
#include <time.h>
#include <arpa/inet.h> //for sockaddr_in and inet_addr()
#include <string.h>
#include <unistd.h> //for close

#include "Param.h"

struct RRC_SETUP_REQUEST _rrc_set_up_request;
// typedef struct param_t
// {
//     int sock_fd;
//     struct OTRACE_DATA_RRC_SETUP_REQ rrc_data_setup_req;
// } param;

char flag = 0;

void *send_trace_data(void *sockfd_p)
{

    char buffer[1024];
    int *sockfd = (int *)sockfd_p;

    // memcpy(buffer, a->rrc_data_setup_req, sizeof(struct OTRACE_DATA_RRC_SETUP_REQ));
    struct OTRACE_DATA_RRC_SETUP_REQ rrc_data_setup_req;
    rrc_data_setup_req.header.msg_type = MSG_DATA;
    rrc_data_setup_req.header.trace_type = TRACE_INTERFACE_Uu;
    rrc_data_setup_req.header.cell_id = 1;
    rrc_data_setup_req.header.data_length = sizeof(struct OTRACE_DATA_RRC_SETUP_REQ);
    rrc_data_setup_req.header.subscriber_id = 11111;
    while (1)
    {
        rrc_data_setup_req.data = _rrc_set_up_request;
        memcpy(buffer, &rrc_data_setup_req, sizeof(struct OTRACE_DATA_RRC_SETUP_REQ));
        if (send(*sockfd, (void *)(&buffer), sizeof(buffer), 0) < 0)
        {
            perror("send respond fail\n");
        }
        sleep(2);
    }

    // rrc_data_setup_req.data = _rrc_set_up_request;
    // memcpy(buffer, &rrc_data_setup_req, sizeof(struct OTRACE_DATA_RRC_SETUP_REQ));
    // if (send(*sockfd, (void *)(&buffer), sizeof(buffer), 0) < 0)
    // {
    //     perror("send respond fail\n");
    // }
    pthread_exit(0);
    // printf("\n%d\n", *sockfd);
}

void *socket_sever(void *port)
{
    // printf("Thread socket sever L3_RRC run !\n\n");
    int listenfd = -1, connfd = -1;
    struct sockaddr_in server_addr, client_addr;
    char send_buffer[1024];
    char recv_buffer[1024];
    time_t ticks;

    memset(send_buffer, 0, sizeof(send_buffer));
    memset(recv_buffer, 0, sizeof(recv_buffer));

    listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);

    if (listenfd < 0)
    {
        perror("error on creating socket");
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(*(int *)port);

    bind(listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(listenfd, 10);

    while (1)
    {
        socklen_t addr_size;
        addr_size = sizeof(struct sockaddr_in);
        connfd = accept(listenfd, (struct sockaddr *)&client_addr, &addr_size);
        // printf("connfd = %d\n",connfd);
        // printf("client port: %d\n",ntohs(client_addr.sin_port));

        /*------------recv request from OTRACE--------------- */
        if (recv(connfd, (void *)(&recv_buffer), sizeof(recv_buffer), 0) < 0)
        {
            perror("recive request fail\n");
        }
        else
        {
            struct RRC_OTRACE_START_REQ recv_cmd;
            memcpy((void *)&recv_cmd, (void *)recv_buffer, sizeof(recv_cmd));
            printf("Recive data msg_type:");
            if (recv_cmd.msg_type == MSG_START_REQ)
                printf("\tMSG_START_REQ\n");
            printf("Recive data trace_type:");
            if (recv_cmd.trace_type == TRACE_INTERFACE_Uu)
                printf("\tTRACE_INTERFACE_Uu\n");
            printf("Recive data cell_id: %d\n", recv_cmd.cell_id);
            printf("Recive data subscriber_id: %d\n", recv_cmd.subscriber_id);
            printf("Recive data duration: %d seconds\n", recv_cmd.trace_duration);
            // ---------------send respond to OTRACE-----------
            if (recv_cmd.msg_type == MSG_START_REQ)
            {
                struct RRC_OTRACE_START_RESP _RRC_OTRACE_START_RESP = {
                    MSG_START_RESP,
                    SUCCESS, // FAIL or SUCCESS
                    // 100 // 100 second for trace, then release
                };
                memcpy(send_buffer, &_RRC_OTRACE_START_RESP, sizeof(_RRC_OTRACE_START_RESP));
                if (send(connfd, (void *)(&send_buffer), sizeof(send_buffer), 0) < 0)
                {
                    perror("send respond fail\n");
                }
                /*------------Duration handler------------*/
                // int duration = recv_cmd.trace_duration;
                // int para[2];
                // para[0] = duration;
                // para[1] = connfd;
                // pthread_t t_duration_handler;
                // pthread_create(&t_duration_handler, NULL, duration_handler, (void *)para);

                /*----------------------------------------*/
                pthread_t t_send_trace_data;
                switch (recv_cmd.trace_type)
                {
                case TRACE_INTERFACE_Uu:
                {
                    // struct OTRACE_DATA_RRC_SETUP_REQ *_otrace_data = (struct OTRACE_DATA_RRC_SETUP_REQ *)malloc(sizeof(struct OTRACE_DATA_RRC_SETUP_REQ));
                    // _otrace_data->header.msg_type = MSG_DATA;
                    // _otrace_data->header.trace_type = TRACE_INTERFACE_Uu;
                    // _otrace_data->header.cell_id = 1;
                    // _otrace_data->header.subscriber_id = 11111;                                  // can xem lai
                    // _otrace_data->header.data_length = sizeof(struct OTRACE_DATA_RRC_SETUP_REQ); // can xem lai
                    // _otrace_data->data = _rrc_set_up_request;

                    memset(send_buffer, 0, sizeof(send_buffer));
                    // memcpy(send_buffer, _otrace_data, sizeof(struct OTRACE_DATA_RRC_SETUP_REQ));
                    // param *_param = (param *)malloc(sizeof(param));
                    // _param->sock_fd = connfd;
                    // _param->rrc_data_setup_req.header.msg_type = MSG_DATA;
                    // _param->rrc_data_setup_req.header.trace_type = TRACE_INTERFACE_Uu;
                    // _param->rrc_data_setup_req.header.cell_id = 1;
                    // _param->rrc_data_setup_req.header.data_length = sizeof(struct OTRACE_DATA_RRC_SETUP_REQ);
                    // _param->rrc_data_setup_req.header.subscriber_id = 11111;
                    // _param->rrc_data_setup_req.data = _rrc_set_up_request;
                    // printf("\n%d\n", connfd);
                    int *ptr_sockfd;
                    ptr_sockfd = &connfd;

                    pthread_create(&t_send_trace_data, NULL, send_trace_data, (void *)ptr_sockfd);
                    // pthread_join(t_send_trace_data, NULL);

                    // if (send(connfd, (void *)(&send_buffer), sizeof(send_buffer), 0) < 0)
                    // {
                    //     perror("send respond fail\n");
                    // }
                    // free(_otrace_data);
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
                /*--------------wait for stop signal------------*/
                memset(recv_buffer, 0, sizeof(recv_buffer));
                if (recv(connfd, (void *)(&recv_buffer), sizeof(recv_buffer), 0) < 0)
                {
                    perror("recive request fail\n");
                }
                else if (*recv_buffer == MSG_STOP_REQ)
                {
                    printf("\nMSG_STOP_REQ\n");
                    // free(_otrace_data);
                    // pthread_exit(0);
                    pthread_cancel(t_send_trace_data);
                    memset(recv_buffer, 0, sizeof(recv_buffer));
                    struct RRC_OTRACE_STOP_RESP _rrc_otrace_stop_resp;
                    _rrc_otrace_stop_resp.msg_type = MSG_STOP_RES;
                    _rrc_otrace_stop_resp.Duration = TRACE_DURATION;
                    _rrc_otrace_stop_resp.Total_msg_trace = 1;
                    memcpy(send_buffer, &_rrc_otrace_stop_resp, sizeof(_rrc_otrace_stop_resp));
                    if (send(connfd, (void *)(&send_buffer), sizeof(send_buffer), 0) < 0)
                    {
                        // printf("abcxyz\n");
                        perror("send reponse fail\n");
                    }
                    close(connfd);
                }

                // pthread_join(t_duration_handler, NULL);
            }
        }
        close(connfd);
    }
    close(listenfd);
    pthread_exit(NULL);
}
void *socket_UE(void *port)
{
    int listenfd = -1, connfd = -1;
    struct sockaddr_in sever_addr, client_addr;
    char buffer[1024];

    memset(buffer, 0, sizeof(buffer));

    listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    sever_addr.sin_family = AF_INET;
    sever_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sever_addr.sin_port = htons(*(int *)port);

    bind(listenfd, (struct sockaddr *)&sever_addr, sizeof(sever_addr));
    listen(listenfd, 10);
    while (1)
    {
        socklen_t addr_size;
        addr_size = sizeof(struct sockaddr_in);
        connfd = accept(listenfd, (struct sockaddr *)&client_addr, &addr_size);
        // printf("connfd = %d\n",connfd);
        /*wait for RRC setup request from UE*/
        while (1)
        {
            int a = recv(connfd, (void *)(&buffer), sizeof(buffer), 0);
            if (a < 0)
            {
                perror("recive request fail\n");
            }

            else if (a > 0)
            {
                char *index;
                index = buffer;
                _rrc_set_up_request.header.Subscription_ID = *index;
                _rrc_set_up_request.header.Misc_ID = *(index + 1);
                _rrc_set_up_request.header.Pkt_Version = *(index + 2);
                _rrc_set_up_request.header.RRC_release_Number_Major_minor = *((uint16_t *)(index) + 2);
                _rrc_set_up_request.header.Radio_Bearer_ID = *((uint16_t *)(index) + 3);
                _rrc_set_up_request.header.Physical_Cell_ID = *((uint16_t *)(index) + 4);
                _rrc_set_up_request.header.NR_Cell_Global_ID = *(index + 10);
                _rrc_set_up_request.header.Freq = *((int *)(index) + 3);
                _rrc_set_up_request.header.sfn = *((uint16_t *)(index) + 8);
                _rrc_set_up_request.header.SubFrameNum = *((uint16_t *)(index) + 9);
                _rrc_set_up_request.header.slot = *((uint16_t *)(index) + 10);
                _rrc_set_up_request.header.PDU_Number = *((uint16_t *)(index) + 11);
                _rrc_set_up_request.header.Msg_length = *((uint16_t *)(index) + 12);
                _rrc_set_up_request.header.SIB_Mask_in_SI = *((uint16_t *)(index) + 13);

                _rrc_set_up_request.ue_Identity_randomeVal = *((uint64_t *)(index) + 4); // need write a function for random UE ID
                _rrc_set_up_request.establishmentCause = *((uint8_t *)(index) + 40);
                _rrc_set_up_request.spare = *((uint8_t *)(index) + 41);
            }
        }
    }
    close(listenfd);
    pthread_exit(NULL);
}
int main()
{
    printf("L3_RRC process running ...\n\n");
    pthread_t t_L3_RRC_OTRACE, t_L3_RRC_UE;
    int *ptr_port_OTRACE, *ptr_port_UE;
    int _port_UE = 10001;
    int _port_OTRACE = 10000;
    ptr_port_OTRACE = &_port_OTRACE;
    ptr_port_UE = &_port_UE;
    pthread_create(&t_L3_RRC_OTRACE, NULL, socket_sever, (void *)ptr_port_OTRACE);
    pthread_create(&t_L3_RRC_UE, NULL, socket_UE, (void *)ptr_port_UE);
    pthread_join(t_L3_RRC_OTRACE, NULL);
    pthread_join(t_L3_RRC_UE, NULL);
    printf("L3_RRC process stop!\n");
    return 0;
}
