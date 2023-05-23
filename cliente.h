/*
 * cliente.h
 *
 *  Created on: 22 may 2023
 *      Author: Usuario
 */

#ifndef CLIENTE_H_
#define CLIENTE_H_

class Cliente {
public:
	int id;
	char* nombre;
	char* mail;
	char* contra;

	Cliente();
	Cliente(int id, char* nombre, char* mail, char* contra);
	virtual ~Cliente();
	char* getNombre();
	char* getContra();

};

#endif /* CLIENTE_H_ */
