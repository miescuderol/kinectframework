#pragma once
#include "Escena.h"
#include "Kinect.h"

class NivelPrueba : public Escena, public ListenerJugadorNuevo
{
public:
	NivelPrueba(Sensor * kinect);
	~NivelPrueba();

	void cargar(Escena * nivelAnt);

	bool isTerminada();

	void update();

	void updateJugadorNuevo(JugadorID user);

	Estado getEstadoFinal();

	virtual void descargar();

private:

	Sensor * kinect;
	long int ciclos;
	JugadorID jugadorActivo;

};