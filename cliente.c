#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <netdb.h>

void enviar(int connfd);
void* recibir(void* connfd);

int main(int argc, char *argv[]) {

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		printf("Error: No se pudo crear el socket.");
		return 1;
	}

	struct sockaddr_in serv_addr;
	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(5000);

	inet_pton(AF_INET, argv[1], &serv_addr.sin_addr); // Error si IP mal escrita

	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))
			< 0) {
		printf("Error: No se pudo conectar\n");
		return 1;
	}
	pthread_t reci;
	pthread_create(&reci, NULL, &recibir, (void*) &sockfd);
	enviar(sockfd);
	return 0;
}

void* recibir(void *connfd) {
	char recvBuff[150];
	recvBuff[1] = 0;
	do {
		do {
			while (read(*(int*) connfd, recvBuff, (150 * sizeof(recvBuff)) - 1)
					< 1)
				;
		} while (strlen(recvBuff) < 1);
		printf("%s\n", recvBuff);
	} while (1);
}

void enviar(int sock) {
	char sendBuff[150];
	do {
		scanf("%s", sendBuff);
		sendBuff[149] = 0;
		write(sock, sendBuff, sizeof(sendBuff));
	} while (1);
}

