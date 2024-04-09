#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#define SERV_PORT 9527



int main(){

	int cfd;
	int n, ret;
	char buf[BUFSIZ];
	// socket
	cfd = socket(AF_INET, SOCK_DGRAM,0);
	struct sockaddr_in addr;
	memset(&addr, 0 , sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET,"127.0.0.1", &addr.sin_addr.s_addr);

	while(fgets(buf, BUFSIZ, stdin)!=NULL){

			ret = sendto(cfd, buf, strlen(buf), 0, (struct sockaddr*)&addr, sizeof(addr));	
			if(ret == -1){
				printf("sendto error\n");
				exit(1);

			}
			n = recvfrom(cfd, buf, strlen(buf), 0, NULL,0); // 不关心服务器信息
			if(n == -1){
				
					printf("recvfrom error\n");
					exit(1);
			}
			write(STDOUT_FILENO, buf,n);

	}
	close(cfd);
	return 0;

}
