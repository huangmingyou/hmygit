#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <arpa/inet.h>
 
 
#define MAX_BUFFER  1024
 
int main()
{
    int sfd, cfd, len, i;
    struct sockaddr_in saddr[2], caddr;
    int saddr_num=2;
    struct sctp_initmsg initmsg;
    char buff[INET_ADDRSTRLEN];
    char buffer[MAX_BUFFER+1] = "Message ##\n";
 
 
    sfd = socket( AF_INET, SOCK_STREAM, IPPROTO_SCTP );
 
    bzero( (void *)&saddr[0], sizeof(saddr) );
    bzero( (void *)&saddr[1], sizeof(saddr) );
    saddr[0].sin_family = AF_INET;
    saddr[0].sin_addr.s_addr = inet_addr("10.0.1.2");
    saddr[0].sin_port = htons(29008);

    saddr[1].sin_family = AF_INET;
    saddr[1].sin_addr.s_addr = inet_addr("10.0.1.4");
    saddr[1].sin_port = htons(29008);
 
   // bind( sfd, (struct sockaddr *)&saddr, sizeof(saddr) );
    sctp_bindx(sfd,(struct sockaddr *)&saddr,saddr_num,SCTP_BINDX_ADD_ADDR);
 
/* Maximum of 3 streams will be available per socket */
    memset( &initmsg, 0, sizeof(initmsg) );
    initmsg.sinit_num_ostreams = 3;
    initmsg.sinit_max_instreams = 3;
    initmsg.sinit_max_attempts = 2;
    setsockopt( sfd, IPPROTO_SCTP, SCTP_INITMSG, 
            &initmsg, sizeof(initmsg) );
 
    listen( sfd, 5 );
 
    for(;;) {
        printf("Server Running\n");
 
        len=sizeof(caddr);
        cfd=accept(sfd, (struct sockaddr *)&caddr, &len);
         
        printf("Connected to %s\n",
            inet_ntop(AF_INET, &caddr.sin_addr, buff,
            sizeof(buff)));
 
         
        for(i=0; i< 3; i++) {
/* Changing 9th character the character after # in the message buffer */
            buffer[9] = '1'+i;
 
            sctp_sendmsg( cfd, (void *)buffer, (size_t)strlen(buffer),
                    NULL, 0, 0, 0, i /* stream */, 0, 0 );
            printf("Sent: %s\n", buffer);
        }
 
        close( cfd );
    }
    return 0;
}
