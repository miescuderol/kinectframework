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

	/** Devuelve la duraci�n del gesto en milisegundos*/
	const long int getDuracion();
	const Articulacion * getArticulacion();

	/** Devuelve el momento (tiempo) en el que se hizo el gesto*/
	const std::time_t getTiempo() const;

	// Setters (hechos de esta forma se pueden encadenar, todos retornan this)
	Gesto * setID(int id);
	Gesto * setPosInicial(XnPoint3D pos);
	Gesto * setPosFinal(XnPoint3D pos);
	Gesto * setTimeInMillis(long int time);
	Gesto * setArticulacion(const XnSkeletonJointTransformation * articulacion);
	
private:

	int id;
	Punto3f posInicial;
	Punto3f posFinal;
	long int timeInMillis;
	const XnSkeletonJointTransformation * articulacion;
};

