#include <stdio.h>
#include <string.h>
#include <time.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
	struct sockaddr_in serv_addr;

	memset(sendBuff, '0', sizeof(sendBuff));
	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(5000);

	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	bind(listenfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));

	listen(listenfd, 10);

	while (1) {
		int connfd = accept(listenfd, (struct sockaddr*) NULL, NULL);
		thread(&procesar, connfd).detach();
	}
	close (connfd);
}

void procesar(int connfd) {
	char recvBuff[1024];
	do {
		bytesRecibidos = read(sockfd, recvBuff, sizeof(recvBuff) - 1);
		recvBuff[bytesRecibidos] = 0;
		printf("%s\n", recvBuff);
		
	} while (strcmp(recvBuff, "Salir"));
}

void enviar_(int connfd) {
	char sendBuff[1024];
	do {
		gets(sendBuff);
		bytesRecibidos = read(sockfd, sendBuff, sizeof(sendBuff));
	} while (strcmp(sendBuff, "Salir"));
}

