#pragma once

#define MSG_START_REQ 1
#define MSG_START_RESP 2
#define MSG_STOP_REQ 3
#define MSG_STOP_RES 4
#define MSG_DATA 5

#define UNLIMITED 0
#define TRACE_ALL 1
#define TRACE_INTERFACE_Uu 2
#define TRACE_INTERFACE_S1AP 3
#define TRACE_INTERFACE_X2AP 3

#define FAIL 0
#define SUCCESS 1

struct RRC_OTRACE_START_REQ
{
    uint8_t msg_type;
    uint8_t trace_type;
    uint8_t cell_id;
    int subscriber_id;
    int trace_duration; //seccond
};

struct RRC_OTRACE_START_RESP
{
    uint8_t msg_type;
    uint8_t resp_type;
    int start_time; //second

};

struct RRC_OTRACE_DATA_Header
{
    uint8_t msg_type;
    uint8_t trace_type;
    uint8_t cell_id;
    int subscriber_id;
    int data_length;
    
};

struct RRC_OTRACE_STOP_REQ
{
    uint8_t msg_type;
};
struct RRC_OTRACE_STOP_RESP
{
    uint8_t msg_type;
    int Duration;
    int Total_msg_trace;
};

struct RRC_SETUP_HEADER
{
    uint8_t Subscription_ID;
    uint8_t Misc_ID;
    uint8_t Pkt_Version;
    uint16_t RRC_release_Number_Major_minor;
    uint16_t Radio_Bearer_ID;
    uint16_t Physical_Cell_ID;
    uint16_t NR_Cell_Global_ID;
    uint16_t Freq;
    uint16_t sfn;
    uint16_t SubFrameNum;
    uint16_t slot;
    uint16_t PDU_Number;
    uint16_t Msg_length;
    uint16_t SIB_Mask_in_SI;
};

struct RRC_SETUP_REQUEST{
    struct RRC_SETUP_HEADER header;
    uint64_t ue_Identity_randomeVal:39;
    char establishmentCause:1;
    char spare:1;

};

struct OTRACE_DATA{
    RRC_OTRACE_DATA_Header header;
    
} ;