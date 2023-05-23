/*
 * pelicula.cpp
 *
 *  Created on: 23 may 2023
 *      Author: Usuario
 */

#include "pelicula.h"
#include "string.h"

Pelicula::Pelicula() {
	this->id_pelicula=0;
		this->titulo=NULL;
		this->cod_genero=0;
		this->director=NULL;
		this->cod_formato=0;
		this->precio=0;
		this->cantidad=0;

}
Pelicula::Pelicula(int id_pelicula, char *titulo, int cod_genero,
			char *director, int cod_formato, float precio, int cantidad){
	this->id_pelicula=id_pelicula;
	this->titulo=titulo;
	this->cod_genero=cod_genero;
	this->director=director;
	this->cod_formato=cod_formato;
	this->precio=precio;
	this->cantidad=cantidad;
}

Pelicula::~Pelicula() {
	// TODO Auto-generated destructor stub
}

