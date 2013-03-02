#pragma once
#include <boost/thread.hpp>
#include "Wrappers.h"

class Gesto
{
public:

	Gesto();
	~Gesto();

	// Getters
	const std::string getNombre();
	const Punto3f getPosInicial();
	const Punto3f getPosFinal();

	
	const long int getDuracion();
	/*!< Devuelve la duración del gesto en milisegundos*/
	const std::string getArticulacion();

	const std::time_t getTiempo() const;
	/*!< Devuelve el momento (tiempo) en el que se hizo el gesto*/

	// Setters (hechos de esta forma se pueden encadenar, todos retornan this)
	Gesto * setNombre(std::string nombre);
	Gesto * setPosInicial(Punto3f pos);
	Gesto * setPosFinal(Punto3f pos);
	Gesto * setTimeInMillis(long int time);
	Gesto * setArticulacion(std::string idJugador_articulacion);
	Gesto * setTiempo(std::time_t t);
	
private:

	std::string nombre;
	Punto3f posInicial;
	Punto3f posFinal;
	long int timeInMillis;
	std::time_t tiempo;
	std::string articulacion;

};

