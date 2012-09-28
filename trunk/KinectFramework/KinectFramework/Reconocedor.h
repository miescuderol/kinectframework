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
	Reconocedor(GestoPatron * gestoPatron, XnSkeletonJointTransformation * articulacion, std::vector<ReconocedorBasico*> * recBasicos);
	~Reconocedor(void);

	Gesto * lastGesture();
	void addListener(ListenerGesto * listener);
	void update(Movimiento * m);

private:

	void notifyObservers();
	std::vector<ListenerGesto*> listeners;

	std::vector<ReconocedorBasico*> * recBasicos;
	Gesto * ultimoGesto;
	GestoPatron * gestoTemplate;
	Movimiento * movimientoEsperado;
	int ultimoMovimiento;

};

