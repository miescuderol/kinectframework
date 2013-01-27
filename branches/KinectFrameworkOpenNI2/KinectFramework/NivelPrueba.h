#pragma once
#include "Escena.h"
#include "Kinect.h"

class NivelPrueba : public Escena, public ListenerJugadorNuevo
{
public:
	NivelPrueba(Sensor * kinect);
	~NivelPrueba();

	void cargar(Escena * nivelAnt);

	bool isTerminado();

	void update();

	void updateJugadorNuevo(JugadorID user);

	Estado getEstadoFinal();

private:

	Sensor * kinect;
	long int ciclos;
	JugadorID jugadorActivo;

};