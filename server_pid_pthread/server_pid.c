#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include <ctype.h>
#include <signal.h>
#include <sys/wait.h>
#define SEV_PORT 9527

void catch_child(int signum){
	while(waitpid(0,NULL,WNOHANG) > 0);
	return;
}

int main(){
	int ret;
	int sfd, cfd;
	struct sockaddr_in saddr, caddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(SEV_PORT);
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// 1. 服务端socket
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sfd == -1){
		perror("socket error\n");
		exit(1);
	}
	// 2. 服务端bind 地址结构体
	ret = bind(sfd, (struct sockaddr*)&saddr, sizeof(saddr));
	if(ret == -1){
		perror("bind error\n");
		exit(1);
	}
	// 3.服务端listen设置最大同时连接
	listen(sfd,128);
	socklen_t caddr_len;
	caddr_len = sizeof(caddr);
	// 4.服务端accept循环阻塞等待客户端连接
	while(1){
		cfd = accept(sfd, (struct sockaddr*)&caddr,&caddr_len);	
		if(cfd == -1){
				perror("accept error\n");
				exit(1);
		}
		pid_t pid = fork();
		// 子进程工作
		if(pid == 0){
			char buf[BUFSIZ];
			close(sfd); // 子进程关掉接客套接字
			while(1){
				ret = read(cfd, buf, sizeof(buf));
				if(ret == 0){
					close(cfd);
					exit(0);
				}
				for(int i =0; i < ret ; i++ ){
					buf[i] = toupper(buf[i]);
				}
				write(cfd, buf, ret);
			}
		}
		// 父进程通过信号回收子进程
		else{
			close(cfd);
			//注册捕捉函数
			struct sigaction act;
			act.sa_handler = catch_child;
			sigemptyset(&act.sa_mask);
			act.sa_flags = 0;
			sigaction(SIGCHLD,&act, NULL);
		}
	}
	return 0;
}
