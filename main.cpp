#include <stdio.h>
#include <winsock2.h>
#include <math.h>
#include "cliente.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

Cliente* cargarClientes(){
	Cliente* listaClientes = new Cliente[10];
	Cliente cliente(1,"zzz","123","123");
	listaClientes[0]=cliente;
	return listaClientes;
}

int main(int argc, char *argv[]) {

	WSADATA wsaData;
	SOCKET conn_socket;
	SOCKET comm_socket;
	struct sockaddr_in server;
	struct sockaddr_in client;
	char sendBuff[512], recvBuff[512];

	printf("\nInitialising Winsock...\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("Failed. Error Code : %d", WSAGetLastError());
		fflush(stdout);
		return -1;
	}

	printf("Initialised.\n");
	fflush(stdout);

	//SOCKET creation
	if ((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("Could not create socket : %d", WSAGetLastError());
		fflush(stdout);
		WSACleanup();
		return -1;
	}

	printf("Socket created.\n");
	fflush(stdout);

	server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

	//BIND (the IP/port with socket)
	if (bind(conn_socket, (struct sockaddr*) &server,
			sizeof(server)) == SOCKET_ERROR) {
		printf("Bind failed with error code: %d", WSAGetLastError());
		fflush(stdout);
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}

	printf("Bind done.\n");
	fflush(stdout);

	//LISTEN to incoming connections (socket server moves to listening mode)
	if (listen(conn_socket, 1) == SOCKET_ERROR) {
		printf("Listen failed with error code: %d", WSAGetLastError());
		fflush(stdout);
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}

	//ACCEPT incoming connections (server keeps waiting for them)
	printf("Waiting for incoming connections...\n");
	fflush(stdout);
	int stsize = sizeof(struct sockaddr);
	comm_socket = accept(conn_socket, (struct sockaddr*) &client, &stsize);
	// Using comm_socket is able to send/receive data to/from connected client
	if (comm_socket == INVALID_SOCKET) {
		printf("accept failed with error code : %d", WSAGetLastError());
		fflush(stdout);
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}
	printf("Incomming connection from: %s (%d)\n", inet_ntoa(client.sin_addr),
			ntohs(client.sin_port));
	fflush(stdout);
	// Closing the listening sockets (is not going to be used anymore)
	closesocket(conn_socket);

	//SEND and RECEIVE data (CLIENT/SERVER PROTOCOL)
	printf("Waiting for incoming commands from client... \n");
	fflush(stdout);
	do {
		recv(comm_socket, recvBuff, sizeof(recvBuff), 0);

		printf("Command received: %s \n", recvBuff);
		fflush(stdout);
		if (strcmp(recvBuff, "INICIARSESION") == 0) {
			fflush(stdout);
		}
		if (strcmp(recvBuff, "INICIARSESION") == 0) {
			int contador = 0;
			int correcto = 0;
			char nombre[20];
			char contra[20];
			Cliente* listaClientes = cargarClientes();
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			while (strcmp(recvBuff, "SESIONEND") != 0) {
				if (contador == 0) {
					strcpy(nombre, recvBuff);

				}
				else if (contador == 1) {
					strcpy(contra, recvBuff);

				}
				recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
				contador++;
			}
			//int numClientes = contarClientes();
			for (int i = 0; i < 1; ++i) {
				if (strcmp(listaClientes[i].getNombre(), nombre) == 0 && strcmp(listaClientes[i].getContra(), contra) == 0) {
					correcto = 1;
					break;  // Se encontró un cliente correcto, no es necesario seguir iterando
				}
			}
			printf("Response sent: %d \n", correcto);
			fflush(stdout);
			sprintf(sendBuff, "%d", correcto);
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			printf("Response sent: %s \n", sendBuff);
			fflush(stdout);
		}

		//if (strcmp(recvBuff, "EXIT") == 0)
			//break;

	} while (1);



	do
	{
		recv(comm_socket, recvBuff, sizeof(recvBuff), 0);

		printf("Command received: %s \n", recvBuff);
		fflush(stdout);

		if (strcmp(recvBuff, "SUMAR") == 0)
		{
			int suma = 0;
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			while (strcmp(recvBuff, "SUMAR-END") != 0)
			{
				int n = atoi(recvBuff);
				suma += n;
				recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			}
			sprintf(sendBuff, "%d", suma);
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			printf("Response sent: %s \n", sendBuff);
			fflush(stdout);
		}

		if (strcmp(recvBuff, "RAIZ") == 0)
		{
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			int n = atoi(recvBuff);
			float raiz = sqrt(n);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			if (strcmp(recvBuff, "RAIZ-END") == 0); // Nada que hacer

			sprintf(sendBuff, "%f", raiz);
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			printf("Response sent: %s \n", sendBuff);
			fflush(stdout);
		}

		if (strcmp(recvBuff, "IP") == 0)
		{
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			if (strcmp(recvBuff, "IP-END") == 0); // Nada que hacer

			strcpy(sendBuff, inet_ntoa(server.sin_addr));
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			printf("Response sent: %s \n", sendBuff);
			fflush(stdout);
		}

		if (strcmp(recvBuff, "EXIT") == 0)
			break;

	} while (1);

	// CLOSING the sockets and cleaning Winsock...
	closesocket(comm_socket);
	WSACleanup();

	return 0;
}
