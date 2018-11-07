#include <stdio.h>
#include <string.h>
#include <time.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void enviar(int connfd, char* cad);
void recibir(int connfd);
void* procesar(int connfd);

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
		pthread_t thread;
		int connfd = accept(listenfd, (struct sockaddr*) NULL, NULL);
		pthread_create(&thread, NULL, &procesar, connfd);
	}
	close (connfd);
}

void procesar(int connfd) {
	enviar(connfd, "Ingrese nombre de materia");
	char* materia = recibir(connfd);
	char* cad;
	do {
		enviar(connfd,
				"Seleccione la accion a realizar: \n\t1) Ver Promedio\n\t2) Realizar carga\n\t3) Salir");
		cad = recibir(connfd);

		if (strcmp(cad, "3")) {
			close(connfd);
			return;
		}
		// pedir semaforo
		char* tipoPeticion = cad;

		if (strcmp(tipoPeticion, "2")) {

			enviar(connfd, "Ingrese DNI");
			char* dni = recibir(connfd);
			enviar(connfd, "Ingrese tipo de examen");
			char* examen = recibir(connfd);
			char* temp;
			sprintf(temp, "%s;%s;%s;", dni, materia, examen);
			if (strstr(archivo, temp) != -1) {
				char* temp2;
				enviar(connfd, "Ingrese la nota");
				sprintf(temp2, "%s\n%s;%s", archivo, temp, recibir(connfd));
				archivo = temp2;

		enviar(connfd,"Carga realizada con exito");
			} else
				enviar(connfd,
						strcat("ya se realizo la carga del alumno", dni));
		}

		//liberar
	} while (1);
}

char* recibir(int connfd) {
	read(sockfd, recvBuff, sizeof(recvBuff) - 1);
	printf("%s\n", recvBuff);
	return recvBuff;
}

void enviar(int connfd, char* cad) {
	char sendBuff[1024];
	strncpy(cad, sendBuff, 1023);
	sendBuff[1023] = 0;
	gets(sendBuff);
	write(sockfd, sendBuff, sizeof(sendBuff));
}

