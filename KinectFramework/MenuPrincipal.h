#pragma once
#include "escena.h"
#include "Sensor.h"
#include "Rendering.h"
#include "ListenerManoNueva.h"
#include "ListenerManoPerdida.h"
#include "Renderizable.h"


class MenuPrincipal :
	public Escena,
	public ListenerManoNueva,
	public ListenerManoPerdida,
	public Renderizable
{
public:
	MenuPrincipal(Sensor * kinect, Rendering * rendering);
	~MenuPrincipal(void);

	void cargar( Escena * escenaAnt );

	bool isTerminada();

	void update();

	Estado getEstadoFinal();

	void descargar();

	void updateManoNueva( JugadorID mano );

	void updateManoPerdida( JugadorID mano );

	void renderizar();

private:
	Sensor * kinect;
	Rendering * rendering;
	JugadorID idMano;
	const Punto3f * punto;

};

