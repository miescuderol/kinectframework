#include "Movimiento.h"


Movimiento::Movimiento(void)
{
}


Movimiento::~Movimiento(void)
{
}

void Movimiento::setDireccionVertical(Direccion d) {
	vertical = d;
}

void Movimiento::setDireccionHorizontal(Direccion d) {
	horizontal = d;
}

Movimiento::Direccion Movimiento::getDireccionVertical() {
	return vertical;
}

Movimiento::Direccion Movimiento::getDireccionHorizontal() {
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