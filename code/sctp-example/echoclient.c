#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <arpa/inet.h>
 
 

int main(void)
{
	int sfd,cfd,len,i;
	struct sockaddr_in saddr,caddr;
	
	// create socket
	sfd=socket(AF_INET,SOCK_STREAM,IPPROTO_SCTP);
	if (sfd < 0){
		printf("create socket failed!\n");
	}else{
		printf("create socket success!\n");
	}

	bzero((void *)&saddr,sizeof(saddr));
	bzero((void *)&caddr,sizeof(saddr));

	// set socket addr
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr("10.0.1.2");
	saddr.sin_port = htons(3000);


	//bind socket


	//connect
	char buff[1024];
	connect(sfd,(struct sockaddr *)&saddr,sizeof(saddr));
	while(1){
		write(sfd,"hello",5);
		len=read(sfd,(void *)buff,2);
		buff[len+1]='\n';
		printf("%s\n",buff);
		sleep(1);

	}
	
	

	return 0;
}
