#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/select.h>
#define SERVE_PORT 9527
#include <sys/types.h>
#include <arpa/inet.h>

int main(){
	int lfd, cfd;
	int ret, i, len;
	int maxfd;
	char buf[BUFSIZ];
	struct sockaddr_in saddr,caddr;
	socklen_t caddr_len;
	caddr_len = sizeof(caddr);

	//结构体清空
	memset(&saddr, 0, sizeof(saddr));

	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(SERVE_PORT);
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	// 1. socket
	lfd = socket(AF_INET, SOCK_STREAM, 0);
	maxfd = lfd;
	// 2. bind
	bind(lfd, (struct sockaddr*)&saddr, sizeof(saddr));
	// 3. listen
	listen(lfd, 128);

	// 设置监听集合
	fd_set rset, allset;
	FD_ZERO(&allset);
	FD_SET(lfd, &allset); 

	while(1){
		rset = allset;
		// 4.设置监听
		ret = select(maxfd+1, &rset, NULL,NULL,NULL );

		if(ret < 0){
			perror("select error\n");
			exit(1);
		}
		// 5. 监听请求连接	
		if(FD_ISSET(lfd, &rset)){

				cfd = accept(lfd, (struct sockaddr*)&caddr,&caddr_len);
				FD_SET(cfd, &allset);
				if(cfd > maxfd){
					maxfd = cfd;
				}

				if(ret == 1){
					continue;
				}
		}
		
		for(i = lfd+1; i <= maxfd; i++ ){

				if(FD_ISSET(i, &rset)){
					len = read(i, buf, sizeof(buf));
					// 如果i关闭了
					if(len == 0){
						close(i);
						FD_CLR(i, &allset);
					}
					write(STDOUT_FILENO, buf, len);// 把读的打印到屏幕
				}
				
		}

	}

	close(lfd);
	return 0;
}
