/*
 * pelicula.h
 *
 *  Created on: 23 may 2023
 *      Author: Usuario
 */

#ifndef PELICULA_H_
#define PELICULA_H_

class Pelicula {
public:
	int id_pelicula;
	char *titulo;
	int cod_genero;  //crear una tabla en la base de datos con cod_genero y genero
	char *director;
	int cod_formato;  //crear una tabla en la base de datos con cod_formato y formato
	float precio;
	int cantidad;
	Pelicula();
	Pelicula(int id_pelicula, char *titulo, int cod_genero,
			char *director, int cod_formato, float precio, int cantidad);
	virtual ~Pelicula();

};

#endif /* PELICULA_H_ */
