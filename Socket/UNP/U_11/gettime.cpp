/*************************************************************************
	> File Name: gettime.cpp
	> Author: 
	> Mail: 
	> Created Time: 2019年05月02日 星期四 17时56分52秒
 ************************************************************************/

#include "../model.h"
#include <time.h>
#include<iostream>

int main (void) {
	struct sockaddr_in servaddr;
	char buf[1024] = {0};
	int sockfd;
	if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0) 
		ERR_EXIT("socket");
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(13);
	servaddr.sin_addr.s_addr = INADDR_ANY;
	int flags = 1;
	if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&flags,sizeof(flags)) < 0)
		ERR_EXIT("setsockopt");
	if(bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) < 0)
		ERR_EXIT("bind");
	if(listen(sockfd,SOMAXCONN) < 0)
		ERR_EXIT("listen");
	time_t ticks;
	struct sockaddr_in cli;
	memset(&cli,0,sizeof(cli));
	socklen_t len = sizeof(cli);
	int confd;
	while(1) {
	 	if((confd = accept(sockfd,(struct sockaddr*)&cli,&len)) < 0)
			ERR_EXIT("accept");
		ticks = time(NULL);
		snprintf(buf,sizeof(buf),"%.24s\r\n",ctime(&ticks));
		write(confd,buf,strlen(buf));
		memset(buf,0,sizeof(buf));
		close(confd);
	}
	close(sockfd);



}

