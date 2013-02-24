#pragma once
#include "Escena.h"
#include "Kinect.h"

class NivelPrueba : public Escena, 
					public ListenerJugadorNuevo, 
					public ListenerJugadorPerdido, 
					public ListenerJugadorCalibrado,
					public ListenerGesto
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

	void updateJugadorPerdido( JugadorID user );

	void updateJugadorCalibrado( JugadorID user );

	void updateGesto( Gesto * m );

private:

	Sensor * kinect;
	long int ciclos, ciclosGesto;
	JugadorID jugadorActivo;
	std::vector<JugadorID> jugadores;
	bool gestoDetectado;

};