#pragma once
#include "Nivel.h"
#include "Kinect.h"

class NivelPrueba : public Nivel, public ListenerJugadorNuevo
{
public:
	NivelPrueba(Kinect * kinect);
	~NivelPrueba();

	void cargar(Nivel * nivelAnt);

	bool isTerminado();

	void update();

	void updateJugadorNuevo(XnUserID user);

	Estado getEstadoFinal();

private:

	Kinect * kinect;
	long int ciclos;
	XnUserID jugadorActivo;

};