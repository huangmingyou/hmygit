#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
 
#define MAX_BUFFER  1024
 
int main(int argc, char **argv)
{
    int cfd, i, flags;
    struct sockaddr_in saddr;
    struct sctp_sndrcvinfo sndrcvinfo;
    struct sctp_event_subscribe events;
    struct sctp_initmsg initmsg;
    char buffer[MAX_BUFFER+1];
 
    if(argc!=2) {
        printf("Usage: %s ipaddress\n", argv[0]);
    return -1;
    }
 
    cfd = socket( AF_INET, SOCK_STREAM, IPPROTO_SCTP );
 
/* Specify that a maximum of 3 streams will be available per socket */
    memset( &initmsg, 0, sizeof(initmsg) );
    initmsg.sinit_num_ostreams = 3;
    initmsg.sinit_max_instreams = 3;
    initmsg.sinit_max_attempts = 2;
    setsockopt( cfd, IPPROTO_SCTP, SCTP_INITMSG,
            &initmsg, sizeof(initmsg) );
 
    bzero( (void *)&saddr, sizeof(saddr) );
    saddr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &saddr.sin_addr);
    saddr.sin_port = htons(29008);
     
    connect( cfd, (struct sockaddr *)&saddr, sizeof(saddr) );
 
    memset( (void *)&events, 0, sizeof(events) );
    events.sctp_data_io_event = 1;
    setsockopt( cfd, SOL_SCTP, SCTP_EVENTS,
        (const void *)&events, sizeof(events) );
 
     
/* Sending three messages on different streams */
 
    for (i=0; i<3; i++) {
        bzero( (void *)&buffer, sizeof(buffer) );
 
        sctp_recvmsg( cfd, (void *)buffer, sizeof(buffer),
                        (struct sockaddr *)NULL, 0, &sndrcvinfo, &flags );
 
        printf("Received following data on stream %d\n\n%s\n",
            sndrcvinfo.sinfo_stream, buffer);
 
    }
 
    close(cfd);
 
    return 0;
}
