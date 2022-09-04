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
#define TRACE_INTERFACE_X2AP 4

#define RRC_setup_req 1
#define RRC_setup 2
#define RRC_setup_complete 3
#define RRC_realease 4


#define FAIL 0
#define SUCCESS 1

/*-----define param for simulation-------*/
#define sub_id 1
#define misc_id 0
#define ptk_version 12
#define RRC_release_number_major_minor 15100
#define Radio_bearer_id 0
#define PCI 422
#define NR_cell_global_ID_ 3
#define Freq_ 500910
#define SFN 88
#define SubFramNum 3
#define SLOT 77
#define PDU_number 55
#define Msg_length_ 6
#define SIB_Mask_in_SI_ 0x0C

enum EstablishmentCause  {emergency, highPriorityAccess, mt_Access, mo_Signalling, mo_Data, mo_VoiceCall, mo_VideoCall,
mp_SMS, mps_PriorityAccess, mcs_PriorityAccess, spare6, spare5, spare4, spare3, spare2, spare1};
// #define mo_Signalling 1
/*---------------------------------------*/
struct RRC_OTRACE_START_REQ
{
    uint8_t msg_type;
    uint8_t trace_type;
    uint8_t cell_id;
    int subscriber_id;
    int trace_duration; //second measurement unit
};

struct RRC_OTRACE_START_RESP
{
    uint8_t msg_type;
    uint8_t resp_type;
    // int start_time;
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
    int Freq;
    uint16_t sfn;
    uint16_t SubFrameNum;
    uint16_t slot;
    uint16_t PDU_Number;
    uint16_t Msg_length;
    uint16_t SIB_Mask_in_SI;
};

struct RRC_SETUP_REQUEST{
    struct RRC_SETUP_HEADER header;
    // uint64_t ue_Identity_randomeVal:39;
    // uint8_t establishmentCause:7;
    // char spare:1;
    uint64_t ue_Identity_randomeVal;
    uint8_t establishmentCause;
    uint8_t spare;
};


struct OTRACE_DATA_RRC_SETUP_REQ{
    struct RRC_OTRACE_DATA_Header header;
    struct RRC_SETUP_REQUEST data;
} ;

