#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <math.h>
#include "cliente.h"
#include "pelicula.h"

//#include "sqlite3.h"

extern "C"{
	#include "sqlite3.h"
}

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

	sqlite3 *db;
	sqlite3_stmt *stmt;
	sqlite3_stmt *stmt2;
	sqlite3_stmt *stmt3;


	int result;


	// METODOS BASES DE DATOS

char* load_config(char* filename, char* buscar) {
	FILE* archivo;
	    char linea[100];
	    char* igual;
	    char buscar2[100];

	    archivo = fopen(filename, "r");

	    if (archivo == NULL) {
	        printf("Error al abrir el archivo.\n");
	        return NULL;
	    }

	    char* resultado = NULL;
	    while (fgets(linea, 100, archivo)) {
	        int i = 0;
	        while (linea[i] != '=') {
	            buscar2[i] = linea[i];
	            i++;
	        }
	        buscar2[i] = '\0';

	        if (strcmp(buscar, buscar2) == 0) {
	            igual = strchr(linea, '=');
	            if (igual != NULL) {
	                int longitud = strlen(igual + 1);
	                resultado = (char*) realloc(resultado, longitud * sizeof(char));
	                if (resultado == NULL) {
	                    printf("Error al asignar memoria.\n");
	                    return NULL;
	                }
	                strcpy(resultado, igual + 1);

	            }
	        }
	    }



	    fclose(archivo);
	    return resultado;
}


void inicializarBDD(){

	char* rutaBDD_server = load_config("../videoclub_prog4-master/sql/prueba.txt","rutaBDD_server");


		    sqlite3_open(rutaBDD_server, &db);
		    printf("base de datos inicializada \n");
		    free(rutaBDD_server);

}

void cerrarBDD(){
	sqlite3_close(db);
	printf("base de datos cerrada \n");
}


int contarClientes(){
	char sql[] = "SELECT COUNT(*) FROM Clientes";
		    int count = 0;

		    sqlite3_stmt *stmt2;
		    int result = sqlite3_prepare_v2(db, sql, -1, &stmt2, 0);

		    if (result != SQLITE_OK) {
		        // Manejar el error en la preparación de la consulta
		        return -1;
		    }

		    result = sqlite3_step(stmt2);

		    if (result == SQLITE_ROW) {
		        count = sqlite3_column_int(stmt2, 0);
		    }

		    sqlite3_finalize(stmt2);

		    return count;

}


Cliente* cargarClientes() {
    const char* sql = "SELECT * FROM Clientes";
    Cliente* clientes = new Cliente[contarClientes()];
    int contador = 0;

    sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);

    do {
        result = sqlite3_step(stmt);

        if (result == SQLITE_ROW) {
            Cliente a;
            a.id = sqlite3_column_int(stmt, 0);

            const char* nombre_text = (const char*)sqlite3_column_text(stmt, 1);
            a.nombre = new char[strlen(nombre_text) + 1];
            strcpy(a.nombre, nombre_text);

            const char* mail_text = (const char*)sqlite3_column_text(stmt, 2);
            a.mail = new char[strlen(mail_text) + 1];
            strcpy(a.mail, mail_text);

            const char* contra_text = (const char*)sqlite3_column_text(stmt, 3);
            a.contra = new char[strlen(contra_text) + 1];
            strcpy(a.contra, contra_text);

            a.saldo = sqlite3_column_double(stmt, 4);

            clientes[contador] = a;
            contador++;
        }
    } while (result == SQLITE_ROW);

    sqlite3_finalize(stmt);

    return clientes;
}





//Cliente* cargarClientes() {
//    const char* sql = "select * from Clientes";
//    Cliente* clientes = new Cliente[contarClientes()];
//    int contador = 0;
//
//    sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
//
//    do {
//        result = sqlite3_step(stmt);
//
//        if (result == SQLITE_ROW) {
//        	Cliente a;
//            a.id = sqlite3_column_int(stmt, 0);
//
//            a.nombre = new char[strlen((char*)sqlite3_column_text(stmt, 1)) + 1];
//            strcpy(a.nombre, (char*)sqlite3_column_text(stmt, 1));
//
//            a.mail = new char[strlen((char*)sqlite3_column_text(stmt, 2)) + 1];
//            strcpy(a.mail, (char*)sqlite3_column_text(stmt, 2));
//
//            a.contra = new char[strlen((char*)sqlite3_column_text(stmt, 3)) + 1];
//            strcpy(a.contra, (char*)sqlite3_column_text(stmt, 3));
//
//            a.saldo = sqlite3_column_double(stmt, 4);
//
//            clientes[contador] = a;
//            contador++;
//        }
//    } while (result == SQLITE_ROW);
//
//    sqlite3_finalize(stmt);
//
//    return clientes;
//}


int contarPeliculas(){
	char sql[] = "SELECT COUNT(*) FROM Peliculas";
		    int count = 0;

		    sqlite3_stmt *stmt;
		    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

		    if (result != SQLITE_OK) {
		        // Manejar el error en la preparación de la consulta
		        return -1;
		    }

		    result = sqlite3_step(stmt);

		    if (result == SQLITE_ROW) {
		        count = sqlite3_column_int(stmt, 0);
		    }

		    sqlite3_finalize(stmt);

		    return count;

}
Pelicula* cargarPelis() {
    const char* sql = "select * from Peliculas";
    Pelicula* pelis = new Pelicula[30];
    int contador = 0;

    sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);

    do {
        result = sqlite3_step(stmt);

        if (result == SQLITE_ROW) {
        	Pelicula a;
            a.id_pelicula = sqlite3_column_int(stmt, 0);

            a.titulo = new char[strlen((char*)sqlite3_column_text(stmt, 1)) + 1];
            strcpy(a.titulo, (char*)sqlite3_column_text(stmt, 1));

            a.cod_genero = sqlite3_column_int(stmt, 2);

            a.director = new char[strlen((char*)sqlite3_column_text(stmt, 3)) + 1];
            strcpy(a.director, (char*)sqlite3_column_text(stmt, 3));

            a.cod_formato = sqlite3_column_int(stmt, 4);

            a.precio = sqlite3_column_double(stmt, 5);

            a.cantidad = sqlite3_column_int(stmt, 6);

            pelis[contador] = a;
            contador++;
        }
    } while (result == SQLITE_ROW);

    sqlite3_finalize(stmt);

    printf("peliculas cargadas");
    fflush(stdout);

    return pelis;
}

char* buscarGenero (int cod_gen)
{
	char* gen;

	char sql[] = "select Nombre_Gen from Generos where Cod_Gen = ?";

	sqlite3_prepare_v2(db, sql, strlen(sql), &stmt2, NULL) ;
	sqlite3_bind_int(stmt2,1,cod_gen);


	result = sqlite3_step(stmt2) ;
	if (result == SQLITE_ROW) {
		//printf("%s\n", (char*) sqlite3_column_text(stmt, 0));
		gen = (char*) sqlite3_column_text(stmt2, 0);

		return gen;

		} else {

			return gen = "Genero no encontrado";
		}

	sqlite3_finalize(stmt2);

}
char* buscarFormato (int cod_for){
	char* form;

		char sql[] = "select Nombre_For from Formato where Cod_For = ?";

		sqlite3_prepare_v2(db, sql, strlen(sql), &stmt2, NULL) ;
		sqlite3_bind_int(stmt2,1,cod_for);

		result = sqlite3_step(stmt2) ;
		if (result == SQLITE_ROW) {
			//printf("%s\n", (char*) sqlite3_column_text(stmt, 0));
			form = (char*) sqlite3_column_text(stmt2, 0);

			return form;

			} else {

				return form = "formato no encontrado";
			}

		sqlite3_finalize(stmt2);
}

float obtenerSaldo(int id_cliente){
	float saldo_cliente = 0;

	char sql[] = "SELECT Saldo FROM Clientes where Id = ?";

	sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL) ;
	sqlite3_bind_int(stmt,1,id_cliente);

	result = sqlite3_step(stmt) ;

	saldo_cliente = sqlite3_column_double(stmt, 0);

	printf("saldo actual = %.2f \n", saldo_cliente);
	fflush(stdout);

	sqlite3_finalize(stmt);

	return saldo_cliente;

}


//void actualizarPrecio(float dinero, int id_cliente){
//	char sql[] = "UPDATE Clientes SET Saldo = ? where Id = ?";
//
//			sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
//			sqlite3_bind_double(stmt, 1, dinero);
//			sqlite3_bind_int(stmt, 2, id_cliente);
//
//			 if (result != SQLITE_OK) {
//			    fprintf(stderr, "Error en la consulta: %s\n", sqlite3_errmsg(db));
//			    fflush(stdout);
//
//			  }
//
//			result = sqlite3_step(stmt);
//
//			if (result != SQLITE_DONE) {
//				fprintf(stderr, "Error en la actualización: %s\n", sqlite3_errmsg(db));
//				fflush(stdout);
//
//			  } else {
//				 printf("titulo actualizado\n");
//				 fflush(stdout);
//
//			  }
//
//			  sqlite3_finalize(stmt);
//
//}



void actualizarSaldo(float dinero, int id_cliente){
	printf("dinero_as= %f \n", dinero);
	printf("id_as= %i \n", id_cliente);

	fflush(stdout);

	char sql[] = "UPDATE Clientes SET Saldo = ? where Id = ?";

			sqlite3_prepare_v2(db, sql, strlen(sql), &stmt2, NULL);
			sqlite3_bind_double(stmt2, 1, dinero);
			sqlite3_bind_int(stmt2, 2, id_cliente);


			 if (result != SQLITE_OK) {
			    fprintf(stderr, "Error en la consulta: %s\n", sqlite3_errmsg(db));
			    fflush(stdout);

			  }

			result = sqlite3_step(stmt2);

			if (result != SQLITE_DONE) {
				fprintf(stderr, "Error en la actualización: %s\n", sqlite3_errmsg(db));
				fflush(stdout);


			  } else {
				 printf("saldo actualizado\n");
				 fflush(stdout);


			  }


			  sqlite3_finalize(stmt2);


}


void actualizarCantidad(int cantidad, int id_peli){
	printf("cantidad_ac= %i \n", cantidad);
	printf("idPeli_ac= %i \n", id_peli);
	fflush(stdout);
	char sql[] = "UPDATE Peliculas SET Cantidad = ? where Id_Pelicula = ?";

			sqlite3_prepare_v2(db, sql, strlen(sql), &stmt3, NULL);
			sqlite3_bind_int(stmt3, 1, cantidad);
			sqlite3_bind_int(stmt3, 2, id_peli);

			 if (result != SQLITE_OK) {
			    fprintf(stderr, "Error en la consulta: %s\n", sqlite3_errmsg(db));

			  }

			result = sqlite3_step(stmt3);

			if (result != SQLITE_DONE) {
				fprintf(stderr, "Error en la actualización: %s\n", sqlite3_errmsg(db));

			  } else {
				 printf("cantidad actualizada\n");

			  }
			 fflush(stdout);

			  sqlite3_finalize(stmt3);

}

int comprobarCompras(int id_cliente, int id_peli){

	char sql[] = "SELECT COUNT(*) FROM Compras where Id_Pelicula = ? and Id = ?";
			    int count = 0;

			    sqlite3_stmt *stmt;
			    int result = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, 0);
			    sqlite3_bind_double(stmt2, 1, id_peli);
			    sqlite3_bind_int(stmt2, 2, id_cliente);

			    if (result != SQLITE_OK) {
			        // Manejar el error en la preparación de la consulta
			        return -1;
			    }

			    result = sqlite3_step(stmt);

			    if (result == SQLITE_ROW) {
			        count = sqlite3_column_int(stmt, 0);
			    }

			    sqlite3_finalize(stmt);

			    return count;

}

void insertarCompra(int id_p, int id_c, int cant){
	char sql[] = "insert into Compras (Id, Id_Pelicula, Cantidad"
				") values ( ?,?,?)";

		sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL) ;

		sqlite3_bind_int(stmt,1,id_c);
		sqlite3_bind_int(stmt,2,id_p);
		sqlite3_bind_int(stmt,3,cant);




		 if (result != SQLITE_OK) {
			    fprintf(stderr, "Error en la consulta: %s\n", sqlite3_errmsg(db));

			  }

			result = sqlite3_step(stmt);



			if (result != SQLITE_DONE) {
				fprintf(stderr, "Error en la insercion: %s\n", sqlite3_errmsg(db));

			  } else {
				 printf("pelicula insertada\n");

			  }

			  sqlite3_finalize(stmt);



}

int contarCantidadCompra(int id_c, int id_p){
	int cantidad = 0;

		char sql[] = "SELECT Cantidad FROM Compras where Id = ? and Id_Pelicula = ?";

		sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL) ;
		sqlite3_bind_int(stmt,1,id_c);
		sqlite3_bind_int(stmt,2,id_p);


		result = sqlite3_step(stmt) ;

		cantidad = sqlite3_column_int(stmt, 0);


		fflush(stdout);

		sqlite3_finalize(stmt);

		return cantidad;


}

void actualizarCompras(int cantidad, int id_p, int id_c){


	char sql[] = "UPDATE Compras SET Cantidad = ? where Id_Pelicula = ? and Id = ?";

	int viejaCant = contarCantidadCompra(id_c, id_p);
	int nuevaCant = cantidad + viejaCant;

			sqlite3_prepare_v2(db, sql, strlen(sql), &stmt3, NULL);
			sqlite3_bind_int(stmt3, 1, nuevaCant);
			sqlite3_bind_int(stmt3, 2, id_p);
			sqlite3_bind_int(stmt,3,id_c);

			 if (result != SQLITE_OK) {
			    fprintf(stderr, "Error en la consulta: %s\n", sqlite3_errmsg(db));

			  }

			result = sqlite3_step(stmt3);

			if (result != SQLITE_DONE) {
				fprintf(stderr, "Error en la actualización: %s\n", sqlite3_errmsg(db));

			  } else {
				 printf("cantidad actualizada\n");

			  }
			 fflush(stdout);

			  sqlite3_finalize(stmt3);

}
	// FIN METODOS BASES DE DATOS


int main(int argc, char *argv[]) {

	WSADATA wsaData;
	SOCKET conn_socket;
	SOCKET comm_socket;
	struct sockaddr_in server;
	struct sockaddr_in client;
	char sendBuff[512], recvBuff[512];
	FILE* f;

	    char* ruta1 = "../videoclub_prog4-master/sql/prueba.txt";
	    char* ruta2 = "rutaLogger";

	    char*config = load_config(ruta1, ruta2);

	    f = fopen(config, "w");

	printf("\nInitialising Winsock...\n");
	fprintf(f,"%s\n", "\nInitialising Winsock...\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("Failed. Error Code : %d", WSAGetLastError());
		fflush(stdout);
		return -1;
	}

	printf("Initialised.\n");
	fprintf(f,"%s\n", "Initialised.\n");
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

	int id_inicioSesion;
	inicializarBDD();
	do {
		recv(comm_socket, recvBuff, sizeof(recvBuff), 0);

		printf("Command received: %s \n", recvBuff);
		fflush(stdout);
		if (strcmp(recvBuff, "INICIARSESION") == 0) {
			int contador = 0;
			int correcto = 0;
			char nombre[20];
			char contra[20];


//			inicializarBDD();
			Cliente* listaClientes = cargarClientes();




			fflush(stdout);

//			cerrarBDD();

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

//			inicializarBDD();
			int nClientes = contarClientes();
//			cerrarBDD();


			for (int i = 0; i < nClientes; ++i) {
				if (strcmp(listaClientes[i].getNombre(), nombre) == 0 && strcmp(listaClientes[i].getContra(), contra) == 0) {

					id_inicioSesion = listaClientes[i].getId();
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

		if (strcmp(recvBuff, "VERPELIS") == 0)
				{
					printf("verPeliculas activado\n");

//					inicializarBDD();
					Pelicula* listaPelis=cargarPelis();
					int cantidad=contarPeliculas();

					recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
					sprintf(sendBuff, "%d", cantidad);
					send(comm_socket, sendBuff, sizeof(sendBuff), 0);
					for (int i = 0; i < cantidad; ++i) {
						sprintf(sendBuff, "%d", listaPelis[i].id_pelicula);
						send(comm_socket, sendBuff, sizeof(sendBuff), 0);
						sprintf(sendBuff, "%s", listaPelis[i].titulo);
						send(comm_socket, sendBuff, sizeof(sendBuff), 0);
						sprintf(sendBuff, "%s", buscarGenero(listaPelis[i].cod_genero));
						send(comm_socket, sendBuff, sizeof(sendBuff), 0);
						sprintf(sendBuff, "%s", listaPelis[i].director);
						send(comm_socket, sendBuff, sizeof(sendBuff), 0);
						sprintf(sendBuff, "%s", buscarFormato(listaPelis[i].cod_formato));
						send(comm_socket, sendBuff, sizeof(sendBuff), 0);
						sprintf(sendBuff, "%f", listaPelis[i].precio);
						send(comm_socket, sendBuff, sizeof(sendBuff), 0);
						sprintf(sendBuff, "%d", listaPelis[i].cantidad);
						send(comm_socket, sendBuff, sizeof(sendBuff), 0);

//						printf("id = %d \n", listaPelis[i].id_pelicula);
//						printf("titulo = %s \n", listaPelis[i].titulo);
//						printf("genero = %s \n", buscarGenero(listaPelis[i].cod_genero));
//						printf("director = %s \n", listaPelis[i].director);
//						printf("formato = %s \n", buscarFormato(listaPelis[i].cod_formato));
//						printf("precio = %f \n", listaPelis[i].precio);
//						printf("cantidad = %d \n", listaPelis[i].cantidad);

//						printf("peli mandada \n");
						fflush(stdout);
					}
//					cerrarBDD();
					printf("Response sent: enviado \n");
					fflush(stdout);
				}


		if (strcmp(recvBuff, "COMPRARPELIS") == 0)
						{


							int id_peli;
							int cant_peli;
							int saldo_personal;
							int cantidad_actual;
							float precioPeliIndividual;
							float precioPeliTotal;
							float restaDinero;
							int restaCantidad;


							int contador = 0;

							recv(comm_socket, recvBuff, sizeof(recvBuff), 0);

							while (strcmp(recvBuff, "COMPRARPELISEND") != 0) {
								if (contador == 0) {

									sscanf(recvBuff, "%i", &id_peli);
									printf("id_peli= %i \n", id_peli);
									fflush(stdout);


								}
								else if (contador == 1) {
									sscanf(recvBuff, "%i", &cant_peli);
									printf("cantidad_peli= %i \n", cant_peli);
									fflush(stdout);

								}
									recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
									printf("contador= %i\n", contador);
									contador++;
								}



//							inicializarBDD();

							Pelicula* listaPelis = cargarPelis();
							cantidad_actual = listaPelis[id_peli - 1].cantidad;
							printf("cantidad_a = %i \n", cantidad_actual);
							precioPeliIndividual = listaPelis[id_peli - 1].precio;
							printf("Precio_i = %f \n", precioPeliIndividual);
							precioPeliTotal= precioPeliIndividual * cant_peli;
							printf("Precio_t = %f \n", precioPeliTotal);
							saldo_personal = obtenerSaldo(id_inicioSesion);


							fflush(stdout);

//							restaCantidad = cantidad_actual - cant_peli;
//							restaDinero = saldo_personal - precioPeliTotal;

							if(cantidad_actual>=cant_peli){
								restaCantidad = cantidad_actual - cant_peli;
								printf("restaCantidad= %i \n", restaCantidad);
								fflush(stdout);
								if(saldo_personal >= precioPeliIndividual){


									restaDinero = saldo_personal - precioPeliTotal;
									printf("restaDinero= %f \n", restaDinero);
									fflush(stdout);
									actualizarSaldo(restaDinero, id_inicioSesion);
									actualizarCantidad(restaCantidad, id_peli);
									int comprobacion = comprobarCompras(id_inicioSesion, id_peli);
									if (comprobacion == 1){

										actualizarCompras(cant_peli, id_peli, id_inicioSesion);

									}else{
										insertarCompra(id_peli, id_inicioSesion, cant_peli);
									}


									sprintf(sendBuff, "%s", "La transaccion ha sido realizada con exito.");
									send(comm_socket, sendBuff, sizeof(sendBuff), 0);


								}else{
									sprintf(sendBuff, "%s", "No hay fondos suficientes en el saldo de su cuenta.");
									send(comm_socket, sendBuff, sizeof(sendBuff), 0);

								}


							}else{
								sprintf(sendBuff, "%s", "No hay stock suficiente, por favor cambie la cantidad.");
								send(comm_socket, sendBuff, sizeof(sendBuff), 0);
							}





//							cerrarBDD();

						}

		if (strcmp(recvBuff, "MOSTRARSALDO") == 0)
								{
			inicializarBDD();
			sprintf(sendBuff, "%f", obtenerSaldo(id_inicioSesion));
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
//			cerrarBDD();

								}
		if (strcmp(recvBuff, "SUMARSALDO") == 0)
								{

			float saldoSuma;
			float saldoTotal;
			float saldoActual;

//			inicializarBDD();
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			sscanf(recvBuff, "%f", &saldoSuma);
			saldoActual = obtenerSaldo(id_inicioSesion);

			saldoTotal = saldoActual + saldoSuma;

			actualizarSaldo(saldoTotal, id_inicioSesion);

			sprintf(sendBuff, "%f", saldoTotal);
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);



//			cerrarBDD();

								}



		//if (strcmp(recvBuff, "EXIT") == 0)
			//break;

	} while (1);


	// CLOSING the sockets and cleaning Winsock...
	cerrarBDD();
	fclose(f);
	closesocket(comm_socket);
	WSACleanup();

	return 0;
}
