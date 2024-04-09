#include <stdio.h>  
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#define SERV_PORT 9527
#include <arpa/inet.h>

int main(){
	int sfd, cfd;
	int ret;
	// 获取套接字
	sfd = socket(AF_INET, SOCK_STREAM,0);
	if(sfd == -1){
			perror("socket error\n");
			exit(1);
	}
	// 初始化addr地址结构体
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(SERV_PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	// 绑定IP端口到服务器socket
	bind(sfd, (struct sockaddr*)&addr,sizeof(addr) );
	//设置最大同时连接
	listen(sfd, 128);
	//阻塞等待客户端连接
	struct sockaddr_in caddr;
	socklen_t caddr_len = sizeof(caddr);
	cfd = accept(sfd, (struct sockaddr*)&caddr, &caddr_len);
	if(cfd== -1){
		perror("accept error\n");
		exit(1);
	}

	// 读写数据
	char buf[BUFSIZ];
	while(1){
		ret = read(cfd, buf, sizeof(buf));
		
		write(STDOUT_FILENO,buf,ret);

		for(int i = 0; i < ret; i++){
			buf[i]= toupper(buf[i]);
		}
		write(cfd, buf, ret);
	}
	close(sfd);
	close(cfd);
	return 0;
}
