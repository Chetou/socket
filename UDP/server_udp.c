#include <ctype.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define SERV_PORT 9527

int main(){

		int socked, n;
		char buf[BUFSIZ];
		// socked
		socked = socket(AF_INET, SOCK_DGRAM,0);
		// bind
		struct sockaddr_in saddr, caddr;
		socklen_t caddr_len;
		caddr_len = sizeof(caddr);

		saddr.sin_addr.s_addr = htonl(INADDR_ANY);
		saddr.sin_port = htons(SERV_PORT);
		bind(socked, (struct sockaddr*)&saddr, sizeof(saddr));	


			
		while(1){

			n = recvfrom(socked, buf, sizeof(buf), 0, (struct sockaddr*)&caddr, &caddr_len);
			if(n == -1){
					printf("rcvdfrom error \n");
			}
			for(int i = 0; i < n; i++){
				
					buf[i] = toupper(buf[i]);
			}
			sendto(socked, &buf, strlen(buf), 0, (struct sockaddr*)&caddr, caddr_len);
		}
		close(socked);
		return 0;
}

