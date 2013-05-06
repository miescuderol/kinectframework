#pragma once
#include "Escena.h"
#include "Kinect.h"
#include "Rendering.h"
#include "Renderizable.h"
#include "World.h"

class NivelPrueba : public Escena, 
					public ListenerJugadorNuevo, 
					public ListenerJugadorPerdido, 
					public ListenerJugadorCalibrado,
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

	void descargar();

	void updateJugadorPerdido( JugadorID user );

	void updateJugadorCalibrado( JugadorID user );

	void setWorld(World * mundo);

	void renderizar();

	void cargarGL();

private:

	Sensor * kinect;
	long int ciclos;
	JugadorID jugadorActivo;
	std::vector<JugadorID> jugadores;
	bool jugadorCalibrado;
	Rendering * rendering;
	World * mundo;
	int ancho, alto;
};