all:
	gcc -o cliente cliente.c -lpthread -lrt
	gcc -o servidor servidor.c -lpthread -lrt