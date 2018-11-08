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

void enviar(int connfd, char* cad);
char* recibir(int connfd);
void* procesar(void* connfd);

char* archivo = (char*) "\0";

int main() {
	struct sockaddr_in serv_addr;
	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(5000);

	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	bind(listenfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));

	listen(listenfd, 10);
	int connfd;
	while (1) {
		pthread_t thread;
		connfd = accept(listenfd, (struct sockaddr*) NULL, NULL);
		pthread_create(&thread, NULL, &procesar, (void*)&connfd);
	}
	close(connfd);
}

void* procesar(void* cad2) {
	int connfd = *((int*)cad2);
	enviar(connfd, (char*) "Ingrese nombre de materia");
	char* materia = recibir(connfd);
	char* cad;
	do {
		enviar(connfd,
				(char*)"Seleccione la accion a realizar: 1) Ver Promedio 2) Realizar carga 3) Salir");
		cad = recibir(connfd);

		if (strcmp(cad, "3")) {
			//close(connfd);
			//return NULL;
		}
		// pedir semaforo
		char* tipoPeticion = cad;

		if (strcmp(tipoPeticion, (char*) "2")) {

			enviar(connfd, (char*) "Ingrese DNI");
			char* dni = recibir(connfd);
			enviar(connfd, (char*) "Ingrese tipo de examen");
			char* examen = recibir(connfd);
			char* temp;
			sprintf(temp, "%s;%s;%s;", dni, materia, examen);
			if (strstr(archivo, temp)) {
				char* temp2;
				enviar(connfd, (char*) "Ingrese la nota");
				sprintf(temp2, "%s\n%s;%s", archivo, temp, recibir(connfd));
				archivo = temp2;

				enviar(connfd, (char*) "Carga realizada con exito");
			} else
				enviar(connfd,
						strcat((char*) "ya se realizo la carga del alumno",
								dni));
		}

		//liberar
	} while (1);
}

char* recibir(int connfd) {
	char* recvBuff = (char*) malloc(100);
	recvBuff[1]=0;
do{		while(read(connfd, recvBuff, sizeof(recvBuff) - 1)<2);
}
	while(strlen(recvBuff)<2);
	printf("se recibe: '%s'\n", recvBuff);
	return recvBuff;
}

void enviar(int connfd, char* cad) {
	char sendBuff[100];
	strncpy(sendBuff, cad, 100);
	sendBuff[100] = 0;
	printf("se envia: '%s'\n", cad);
	write(connfd, sendBuff, sizeof(sendBuff));
}

