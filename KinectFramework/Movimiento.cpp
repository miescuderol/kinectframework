#include "Movimiento.h"


Movimiento::Movimiento(void)
{
	dirZ = SIN_DIRECCION;
	dirY = SIN_DIRECCION;
	dirX = SIN_DIRECCION;
}


Movimiento::~Movimiento(void)
{
}

void Movimiento::setDireccionZ(int d) {
	dirZ = d;
}

void Movimiento::setDireccionY(int d) {
	dirY = d;
}

void Movimiento::setDireccionX(int d) {
	dirX = d;
}

const int Movimiento::getDireccionZ() const {
	return dirZ;
}

const int Movimiento::getDireccionY() const {
	return dirY;
}

const int Movimiento::getDireccionX() const {
	return dirX;
}

bool Movimiento::operator==(Movimiento & otroMovimiento) {
	if (this->getDireccionX() != otroMovimiento.getDireccionX() &&
		this->getDireccionX() != Movimiento::NO_IMPORTA &&
		otroMovimiento.getDireccionX() != Movimiento::NO_IMPORTA)
		return false;

	if (this->getDireccionY() != otroMovimiento.getDireccionY() &&
		this->getDireccionY() != Movimiento::NO_IMPORTA &&
		otroMovimiento.getDireccionY() != Movimiento::NO_IMPORTA)
		return false;

	if (this->getDireccionZ() != otroMovimiento.getDireccionZ() &&
		this->getDireccionZ() != Movimiento::NO_IMPORTA &&
		otroMovimiento.getDireccionZ() != Movimiento::NO_IMPORTA)
		return false;

	return true;
}

bool Movimiento::operator!=(Movimiento & otroMovimiento) {
	return !(*this==otroMovimiento);
}

void Movimiento::setTiempo( std::time_t t ) {
	this->t = t;
}

void Movimiento::setX( float x ) {
	this->x = x;
}

void Movimiento::setY( float y ) {
	this->y = y;
}

void Movimiento::setZ( float z ) {
	this->z = z;
}

const float Movimiento::getX() const {
	return x;
}

const float Movimiento::getY() const {
	return y;
}

const float Movimiento::getZ() const {
	return z;
}

const std::time_t Movimiento::getTiempo() const {
	return t;
}
