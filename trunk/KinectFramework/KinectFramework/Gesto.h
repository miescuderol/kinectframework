#pragma once
#include "XnCppWrapper.h"
#include "XnTypes.h"
#include <boost/thread.hpp>
#

class Gesto
{
public:

	// Getters
	int getID();
	XnPoint3D getPosInicial();
	XnPoint3D getPosFinal();

	/** Devuelve la duración del gesto en milisegundos*/
	long int getDuracion();
	const XnSkeletonJointTransformation * getArticulacion();

	/** Devuelve el momento (tiempo) en el que se hizo el gesto*/
	bool getTiempo() const;

	// Setters (hechos de esta forma se pueden encadenar, todos retornan this)
	Gesto * setID(int id);
	Gesto * setPosInicial(XnPoint3D pos);
	Gesto * setPosFinal(XnPoint3D pos);
	Gesto * setTimeInMillis(long int time);
	Gesto * setArticulacion(const XnSkeletonJointTransformation * articulacion);
	
private:

	int id;
	XnPoint3D posInicial;
	XnPoint3D posFinal;
	long int timeInMillis;
	const XnSkeletonJointTransformation * articulacion;
};

