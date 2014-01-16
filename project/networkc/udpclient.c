#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>

#define MAXBUF 1024

int main(int argc,char *argv[]){
	int udpsocket;
	int returnstatus;
	int addrlen;
	struct sockaddr_in udpclient,udpserver;
	char buf[MAXBUF];

	if(argc < 3){
		fprintf(stderr,"Usage: %s <ip address> <port> \n",argv[0]);
		exit(1);
	}

/* create socket */

	udpsocket = socket(AF_INET,SOCK_DGRAM,0);
	
	if(udpsocket == -1){
		fprintf(stderr,"Could not create a socket!\n");
		exit(1);
	}else{
		printf("Socket create.\n");
	}

/* client address */
/* use INADDR_ANY to use all locale address */

	udpclient.sin_family = AF_INET;
	udpclient.sin_addr.s_addr = INADDR_ANY;
	udpclient.sin_port = 0;

	returnstatus = bind(udpsocket,(struct sockaddr *)&udpclient,sizeof(udpclient));
	
	if(returnstatus == 0){
		fprintf(stderr,"Bind completed!\n");
	}else{
		fprintf(stderr,"Could not bind to address!\n");
		close(udpsocket);
		exit(1);
	}

/* set up the message to be send to the server */

	strcpy(buf,"For Professionals,By Professionals.\n");

/* server address */

	udpserver.sin_family = AF_INET;
	udpserver.sin_addr.s_addr = inet_addr(argv[1]);
	udpserver.sin_port = htons(atoi(argv[2]));

	returnstatus = sendto(udpsocket,buf,strlen(buf)+1,0,(struct sockaddr*)&udpserver,sizeof(udpserver));
	if (returnstatus == -1){
		fprintf(stderr,"Could not send message!\n");
	}else{
		printf("Message snet.\n");
		addrlen=sizeof(udpserver);
		returnstatus = recvfrom(udpsocket,buf,MAXBUF,0,(struct sockaddr*)&udpserver,&addrlen);
	if (returnstatus == -1){
		fprintf(stderr,"Did not receive confirmation!\n");
	}else{
		buf[returnstatus]=0;
		printf("Received:%s\n",buf);
	}
	}


	close(udpsocket);
	return 0;
}
