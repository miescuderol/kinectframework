#pragma once
#include "Gesture.h"
#include "ReconocedorBasico.h"
#include "GestureTemplate.h"
#include "ListenerMovimiento.h"
#include "ListenerGesto.h"

#include <vector>

class Reconocedor : public ListenerMovimiento
{
public:
	Reconocedor(GestureTemplate gestureTemplate);
	~Reconocedor(void);

	Gesture * lastGesture();
	void addListener(ListenerGesto listener);
	void update(Movimiento m);

private:

	void notifyObservers();
	std::vector<ListenerGesto*> listeners;

	std::vector<ReconocedorBasico*> recBasicos;
	Gesture * ultimoGesto;
	GestureTemplate * gestoTemplate;
	Movimiento * movimientoEsperado;
	int ultimoMovimiento;

};

