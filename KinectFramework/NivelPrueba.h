#pragma once
#include "Escena.h"
#include "Kinect.h"
#include "Rendering.h"
#include "Renderizable.h"

class NivelPrueba : public Escena, 
					public ListenerJugadorNuevo, 
					public ListenerJugadorPerdido, 
					public ListenerJugadorCalibrado,
					public ListenerGesto,
					public Renderizable
{
public:
	NivelPrueba(Sensor * kinect, Rendering * rendering);
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

	void getColorFondo( int& r, int& g, int& b );

	void renderizar();

private:

	Sensor * kinect;
	long int ciclos, ciclosGesto;
	JugadorID jugadorActivo;
	std::vector<JugadorID> jugadores;
	bool gestoDetectado;
	Gesto * gesto;
	int idRec, idRec2, cantGestos;
	Rendering * rendering;

};