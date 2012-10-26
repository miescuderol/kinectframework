#pragma once
#include "Gesto.h"
#include "ReconocedorBasico.h"
#include "GestoPatron.h"
#include "ListenerMovimiento.h"
#include "ListenerGesto.h"
#include "XnCppWrapper.h"

#include <vector>


class Reconocedor : public ListenerMovimiento {
public:
	Reconocedor(GestoPatron * gestoPatron, char* idJugadorArt, XnSkeletonJointTransformation * articulacion, ReconocedorBasico * recBasicos);
	~Reconocedor(void);

	Gesto * getUltimoGesto();
	//Kinect::Joint getArticulacion();
	GestoPatron * getGestoPatron();
	//XnUserID getIDJugador();
	char * getIDJugadorArt();

	void addListener(ListenerGesto * listener);
	void updateMovimiento(Movimiento * m);

private:

	void notifyObservers();
	std::vector<ListenerGesto*> listeners;

	ReconocedorBasico * recBasicos;
	Gesto * ultimoGesto;
	GestoPatron * gestoPatron;
	Movimiento * movimientoEsperado;
	int ultimoMovimiento;
	XnSkeletonJointTransformation * articulacion;
//	Kinect::Joint joint;
//	XnUserID idJugador;
	char * idJugadorArt;

};

