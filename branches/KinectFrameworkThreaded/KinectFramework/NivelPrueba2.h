#pragma once
#include "Nivel.h"
#include "Kinect.h"

class NivelPrueba2 : public Nivel, public ListenerJugadorNuevo
{
public:
	NivelPrueba2(Kinect * kinect);
	~NivelPrueba2();

	void cargar(Nivel * nivelAnt);

	bool isTerminado();

	void update();

    void update(XnUserID user);

	Estado getEstadoFinal();

private:

	Kinect * kinect;
	long int ciclos;
	XnUserID jugadorActivo;

};