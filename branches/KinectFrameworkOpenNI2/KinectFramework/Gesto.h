#pragma once
#include <boost/thread.hpp>
#include "Wrappers.h"

class Gesto
{
public:

	// Getters
	const int getID();
	const Punto3f getPosInicial();
	const Punto3f getPosFinal();

	
	const long int getDuracion();
	/*!< Devuelve la duraci�n del gesto en milisegundos*/
	const Articulacion * getArticulacion();

	
	const std::time_t getTiempo() const;
	/*!< Devuelve el momento (tiempo) en el que se hizo el gesto*/

	// Setters (hechos de esta forma se pueden encadenar, todos retornan this)
	Gesto * setID(int id);
	Gesto * setPosInicial(Punto3f pos);
	Gesto * setPosFinal(Punto3f pos);
	Gesto * setTimeInMillis(long int time);
	Gesto * setArticulacion(const Articulacion * articulacion);
	
private:

	int id;
	Punto3f posInicial;
	Punto3f posFinal;
	long int timeInMillis;
	const Articulacion * articulacion;
};

