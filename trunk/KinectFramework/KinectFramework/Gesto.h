#pragma once
#include "XnCppWrapper.h"

class Gesto
{
public:

	// Getters
	int getID();
	XnPoint3D getPosInicial();
	XnPoint3D getPosFinal();
	long int getTimeInMillis();
	const XnSkeletonJointTransformation * getArticulacion();

	// Setters (hechos de esta forma se pueden encadenar, todos retornan this)
	Gesto * setID(int id);
	Gesto * setPosInicial(XnPoint3D pos);
	Gesto * setPosFinal(XnPoint3D pos);
	Gesto * setTimeInMillis(long int time);
	Gesto * setArticulacion(const XnSkelentonJointTransformation * articulacion);

private:

	int id;
	XnPoint3D posInicial;
	XnPoint3D posFinal;
	long int timeInMillis;
	XnSkeletonJointTransformation * articulacion;

};

