#pragma once
#include "escena.h"
#include "Sensor.h"
#include "Rendering.h"
#include "ListenerJugadorNuevo.h"
#include "ListenerJugadorPerdido.h"
#include "Renderizable.h"


class MenuPrincipal :
	public Escena,
	public ListenerJugadorNuevo,
	public ListenerJugadorPerdido,
	public Renderizable
{

public:

	static const int NINGUNO = 0;
	static const int JUEGO_NUEVO = 1;
	static const int CREDITS = 2;
	static const int SALIR = 3;

	MenuPrincipal(Sensor * kinect, Rendering * rendering);
	~MenuPrincipal(void);

	void cargar( Escena * escenaAnt );

	bool isTerminada();

	void update();

	Estado getEstadoFinal();

	void descargar();

	void renderizar();

	void updateJugadorNuevo( JugadorID user );

	void updateJugadorPerdido( JugadorID user );
	void drawClick();
private:
	Sensor * kinect;
	Rendering * rendering;
	JugadorID idMano;
	const Punto3f * punto;
	GLuint texturaFondo, texturaClick;
	bool clickNeeded;
	int estadoSiguiente;
};

