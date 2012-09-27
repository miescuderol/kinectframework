#include "GestoPatron.h"


GestoPatron::GestoPatron(void)
{
}


GestoPatron::~GestoPatron(void)
{
}

void GestoPatron::addMovement(Movimiento m) {
	movimientos.push_back(m);
}

void GestoPatron::insertAt(int i, Movimiento m) {
	movimientos.insert(movimientos.begin()+i, m);
}

Movimiento GestoPatron::getMovement(int i) {
	return movimientos[i];
}

int GestoPatron::length() {
	return movimientos.size();
}