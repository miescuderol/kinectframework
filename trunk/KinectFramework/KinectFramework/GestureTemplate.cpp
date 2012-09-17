#include "GestureTemplate.h"


GestureTemplate::GestureTemplate(void)
{
}


GestureTemplate::~GestureTemplate(void)
{
}

void GestureTemplate::addMovement(Movimiento m) {
	movimientos.push_back(m);
}

void GestureTemplate::insertAt(int i, Movimiento m) {
	movimientos.insert(movimientos.begin()+i, m);
}

Movimiento GestureTemplate::getMovement(int i) {
	return movimientos[i];
}

int GestureTemplate::length() {
	return movimientos.size();
}