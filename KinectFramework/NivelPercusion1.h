#pragma once
#include "Escena.h"
#include "Kinect.h"
#include "Rendering.h"
#include "Renderizable.h"
#include "World.h"

class NivelPercusion1 : public Escena, 
	public ListenerJugadorNuevo, 
	public ListenerJugadorPerdido, 
	public ListenerJugadorCalibrado,
	public ListenerGesto,
	public Renderizable
{
public:
	NivelPercusion1(Sensor * kinect, Rendering * rendering);
	~NivelPercusion1(void);

	void cargar( Escena * escenaAnt );

	bool isTerminada();

	void update();

	Estado getEstadoFinal();

	void descargar();

	void updateJugadorNuevo( JugadorID user );

	void updateJugadorPerdido( JugadorID user );

	void updateJugadorCalibrado( JugadorID user );

	void renderizar();

	void updateGesto( Gesto * m );
	bool golpeValido();
private:
	Sensor * kinect;
	Rendering * rendering;
	int alto, ancho;

	std::vector<int> puntos;
	float pulso; //en ms
	std::vector<JugadorID> jugadores;
	std::vector<int> idReconocedores;
	int ganador;

	GestoPatron * gestoGolpe;

	double lastTick;
	double delay, errorMargin;

	bool mostrarBoton;

	GLuint texturaBongo;

};

