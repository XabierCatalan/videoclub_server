/*
 * cliente.cpp
 *
 *  Created on: 22 may 2023
 *      Author: Usuario
 */

#include "cliente.h"
#include <iostream>
#include <stdio.h>

Cliente::Cliente() {
	this->id=0;
	nombre=NULL;
	mail=NULL;
	contra=NULL;

}
Cliente::Cliente(int id, char* nombre, char* mail, char* contra){
	this->id=id;
	this->nombre=new char[20];
	this->nombre=nombre;
	this->mail=new char[20];
	this->mail=mail;
	this->contra=new char[20];
	this->contra=contra;
}

Cliente::~Cliente() {
	// TODO Auto-generated destructor stub
}

char* Cliente::getNombre(){
	return this->nombre;
}
char* Cliente::getContra(){
	return this->contra;
}
