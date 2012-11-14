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

Movimiento * GestoPatron::getMovement(int i) {
	return &movimientos[i];
}

const int GestoPatron::length() {
	return movimientos.size();
}

bool GestoPatron::operator==(GestoPatron & otroGesto) {
// TODO: Ver por que no anda poniendo const GestoPatron en el encabezado
	
	if (length() != otroGesto.length())
		return false;

	for (int i = 0; i < length(); i++) {
		if (getMovement(i) != otroGesto.getMovement(i))
			return false;
	}

	return true;

}

bool GestoPatron::operator!=(GestoPatron & otroGesto) {
	return !(*this == otroGesto);
}