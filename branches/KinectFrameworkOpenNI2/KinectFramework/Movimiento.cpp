#include "Movimiento.h"


Movimiento::Movimiento(void)
{
	vertical = SIN_DIRECCION;
	horizontal = SIN_DIRECCION;
}


Movimiento::~Movimiento(void)
{
}

void Movimiento::setDireccionVertical(int d) {
	vertical = d;
}

void Movimiento::setDireccionHorizontal(int d) {
	horizontal = d;
}

int Movimiento::getDireccionVertical() {
	return vertical;
}

int Movimiento::getDireccionHorizontal() {
	return horizontal;
}

bool Movimiento::operator==(Movimiento & otroMovimiento) {
	if (this->getDireccionHorizontal() == otroMovimiento.getDireccionHorizontal() && 
		this->getDireccionVertical() == otroMovimiento.getDireccionVertical())
		return true;
	return false;
}

bool Movimiento::operator!=(Movimiento & otroMovimiento) {
	return !(*this==otroMovimiento);
}