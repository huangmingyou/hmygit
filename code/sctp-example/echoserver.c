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

	i=bind(sfd,(struct sockaddr *)&saddr,sizeof(saddr));
	
	
	if (i < 0){
		printf("bind socket failed!\n");
	}else{
		printf("bind socket success!\n");
	}


	//listen
	listen(sfd,5);
	
	//accept
	
	
	char buff[1024];
	bzero(buff,sizeof(buff));
    while(1)
    {
        cfd = accept(sfd , (struct sockaddr*)NULL, NULL); 
	len=read(cfd,(void *)buff,10);
	printf("read:%d\n",len);
	write(cfd,buff,len);
	printf("%s\n",buff);

     }

	return 0;
}
