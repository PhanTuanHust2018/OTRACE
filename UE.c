#include <stdio.h>
#include <pthread.h>  //for thread 
#include <sys/socket.h> // for socket(), connect(), send(), and recv()
#include <stdlib.h>
#include <time.h>
#include <arpa/inet.h> //for sockaddr_in and inet_addr()
#include <string.h>
#include <unistd.h> //for close

struct RRC_OTRACE
{
    int msg_type;
    int msg_direction;
    int msg_id;
};


void *client_for_L3_gNB(){

}

int main(){
    printf("UE is attaching ...");
    return 0;
}