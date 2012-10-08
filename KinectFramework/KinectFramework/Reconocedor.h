#pragma once
#include "Gesto.h"
#include "ReconocedorBasico.h"
#include "GestoPatron.h"
#include "ListenerMovimiento.h"
#include "ListenerGesto.h"
#include "XnCppWrapper.h"

#include <vector>

class Reconocedor : public ListenerMovimiento
{
public:
	Reconocedor(GestoPatron * gestoPatron, XnUserID * idJugador, XnSkeletonJointTransformation * articulacion, ReconocedorBasico * recBasicos);
	~Reconocedor(void);

	Gesto * getUltimoGesto();
	Kinect::Joint * getArticulacion();
	GestoPatron * getGestoPatron();
	XnUserID * getIDJugador();

	void addListener(ListenerGesto * listener);
	void update(Movimiento * m);

private:

	void notifyObservers();
	std::vector<ListenerGesto*> listeners;

	ReconocedorBasico * recBasicos;
	Gesto * ultimoGesto;
	GestoPatron * gestoPatron;
	Movimiento * movimientoEsperado;
	int ultimoMovimiento;
	XnSkeletonJointTransformation * articulacion;
	XnUserID * idJugador;

};

