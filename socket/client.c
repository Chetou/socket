#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#define SERV_PORT 9527

int main(){
	
	int cfd;
	int ret;
	//套接字
	cfd = socket(AF_INET, SOCK_STREAM, 0);

	// 初始化server_addr
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET, "127.0.0.1",&serv_addr.sin_addr.s_addr);
	// connect连接
	ret = 	connect(cfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if(ret == -1){
		perror("connect error\n");
		exit(1);
	}



	printf("input word\n");
	char buf[BUFSIZ];
	ret = read(STDIN_FILENO,buf, sizeof(buf));

	while(1){
			write(cfd, buf, ret );
			ret = read(cfd, buf, sizeof(buf));
			write(STDOUT_FILENO, buf, ret);
			sleep(1);
	}
	close(cfd);
	return 0;
}
