#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h> // Define constantes de modo
#include <fcntl.h> // Define constantes O_*
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <netdb.h>

int contine(char* archivo, char* temp);
void enviar(int connfd, char* cad);
char* recibir(int connfd);
void* procesar(void* connfd);
sem_t * crearSemaforo(const char *nombre, int valor);
void borrarSemaforo(const char * nombre, sem_t *semaforo);
char* archivo = "\0";
sem_t *sem;
int sEM=0;

int main() {
	sem = crearSemaforo("sem", 1);
	
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
		pthread_create(&thread, NULL, &procesar, (void*) &connfd);
	}
	close(connfd);
	borrarSemaforo("sem");
}

void* procesar(void* cad2) {
	int connfd = *((int*) cad2);
	enviar(connfd, "Ingrese nombre de materia");
	char* materia = recibir(connfd);
	char* cad;
	enviar(connfd,
			"Seleccione la accion a realizar:\n\t1) Ver Promedio\n\t2) Realizar carga\n\t3) Salir");

	do {
		cad = recibir(connfd);

		
		if (!strcmp(cad, "3")) {
			close(connfd);
			return NULL;
		}
		
		if(sEM)
			enviar(connfd, "Se esta realizando una operacion en este momento. Espere unos instantes.\n");
		sem_wait(sem);
		if(sEM)
			enviar(connfd, "Listo. Gracias por aguardar\n");
		sEM=1;
		char* tipoPeticion = cad;

		if (!strcmp(tipoPeticion, "2")) {

			enviar(connfd, "Ingrese DNI");
			char* dni = recibir(connfd);
			enviar(connfd, "Ingrese tipo de examen");
			char* examen = recibir(connfd);
			char temp[100];
			sprintf(temp, "%s;%s;%s", dni, materia, examen);
			if (contine(archivo, temp)) {
				char temp2[110];
				enviar(connfd, "Ingrese la nota");
				sprintf(temp2, "%s;%s", temp, recibir(connfd));
				char temp3[110];
				sprintf(temp3, "%s\n%s",archivo, temp2);
				archivo = temp3;
				FILE* pf=fopen("Notas.txt","at");
				fprintf(pf, "%s\n", temp2);
				fclose(pf);
				printf("%s\n", archivo);
				enviar(connfd,
						"Carga realizada con exito\n\nSeleccione la accion a realizar:\n\t1) Ver Promedio\n\t2) Realizar carga\n\t3) Salir");
			} else
				enviar(connfd,
						"ya se realizo la carga del alumno\n\nSeleccione la accion a realizar:\n\t1) Ver Promedio\n\t2) Realizar carga\n\t3) Salir");
		}
		sEM=0;
		sem_post(sem);
	} while (1);
}

int contine(char* archivo, char* c) {
	int i = 0;
	archivo--;
	while (*(++archivo)) {
		while (*(c + (i)) && *(c + i) == *(archivo + i)) {
			if (!*(c + i + 1))
				return 0;
			i++;
		}
	}
	return 1;
}

char* recibir(int connfd) {
	char* recvBuff = malloc(150);
	recvBuff[1] = 0;
	do {
		while (read(connfd, recvBuff, (150 * sizeof(recvBuff)) - 1) < 1)
			;
	} while (strlen(recvBuff) < 1);
	printf("se recibe: '%s'\n", recvBuff);
	return recvBuff;
}

void enviar(int connfd, char* cad) {
	char sendBuff[150];
	strncpy(sendBuff, cad, 150);
	sendBuff[150] = 0;
	printf("se envia : '%s'\n", cad);
	write(connfd, sendBuff, sizeof(sendBuff));
}


sem_t * crearSemaforo(const char *nombre, int valor){
	return sem_open(nombre, O_CREAT, S_IRUSR | S_IWUSR, valor); // 0600
}

void borrarSemaforo(const char * nombre, sem_t *semaforo)
{
	sem_close(semaforo);
	sem_unlink(nombre);
}
