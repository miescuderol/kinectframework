#pragma once
#include "Nivel.h"
#include "Kinect.h"

class NivelPrueba : public Nivel, public ListenerNuevoJugador
{
public:
	NivelPrueba(Kinect * kinect);
	~NivelPrueba();

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