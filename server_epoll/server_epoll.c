#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <ctype.h>
#define SERV_PORT 9527


int main(){
		int lfd, cfd,sockfd, epfd;	
		int ret, len, i, fd_len;
		char buf[1024];
		struct sockaddr_in saddr, caddr;
		socklen_t caddr_len;
		caddr_len = sizeof(caddr);


		// 1. socket创建lfd
		lfd = socket(AF_INET, SOCK_STREAM, 0);
		// 2. bind绑定服务端地址结构体
		saddr.sin_family = AF_INET;
		saddr.sin_port = htons(SERV_PORT);
		saddr.sin_addr.s_addr = htonl(INADDR_ANY);

		bind(lfd, (struct sockaddr*)&saddr, sizeof(saddr));
		// 3. listen
		listen(lfd, 128);

		// 4. 创建epoll树根
		epfd = epoll_create(1024);
		// 5. 将lfd放入结构体加入树
		struct epoll_event tem, es[128];
		tem.events = EPOLLIN;
		tem.data.fd = lfd;
		ret = epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &tem);
		if(ret < 0 ){
			perror("epoll_ctl error\n");
			exit(1);
		}
		while(1){
				// 6. 循环监听.返回活跃fd数，并且活跃fd所在event都放在es数组中
				fd_len = epoll_wait(epfd, es, 128, -1);
				// 7. 遍历活跃fd
				for(i = 0; i < fd_len; i++){
					sockfd = es[i].data.fd; // 便利es数组，取出数组成员event结构体中的fd
					// 如果lfd活跃，有新客户端连接						
					if(sockfd == lfd){
						
							cfd = accept(lfd, (struct sockaddr*)&caddr,&caddr_len );
							// 设置cfd非阻塞,加入红黑树
							int flag = fcntl(cfd, F_GETFL);
							flag |= O_NONBLOCK;
							fcntl(cfd, F_SETFL, flag);

							tem.events = EPOLLIN | EPOLLET;
							tem.data.fd = cfd;
							ret = epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &tem);
					}
					// 如果非lfd活跃，则是其他cfd有数据可读
					else{
						len = read(sockfd, buf, sizeof(buf));	
						if(len == 0){
							close(sockfd);
							epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, NULL);
						}
						else{
							for(i = 0; i < len; i++){
									buf[i] = toupper(buf[i]);
							}
							write(sockfd, buf, len);
							write(STDOUT_FILENO, buf, len);
						}

					}
				}
				
		}
		return 0;
}
