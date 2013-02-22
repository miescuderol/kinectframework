#pragma once
#include "Escena.h"
#include "Kinect.h"

class NivelPrueba2 : public Escena, public ListenerJugadorNuevo
{
public:
	NivelPrueba2(Sensor * kinect);
	~NivelPrueba2();

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