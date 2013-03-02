#include "ReconocedorBasico.h"
#include "ListenerMovimiento.h"
#include "math.h"
#include <iostream>

#define PI 3.14159

ReconocedorBasico::ReconocedorBasico(float deadZone) {
	this->deadZone = deadZone;
	prevX = prevY = prevZ = 0;
	ultimoMovimiento = new Movimiento();
}

ReconocedorBasico::~ReconocedorBasico(void) {

}

void ReconocedorBasico::setNewPosition(float x, float y, float z) {

	Movimiento * movimiento = new Movimiento();

	float dx = x - prevX;
	float dy = y - prevY;
	float dz = z - prevZ;

	prevX = x;
	prevY = y;
	prevZ = z;

	int movimientoX = 0;
	if (abs(dx) > deadZone)
		movimientoX = dx > 0 ? Movimiento::DERECHA : Movimiento::IZQUIERDA;

	int movimientoY = 0;
	if (abs(dy) > deadZone)
		movimientoY = dy > 0 ? Movimiento::ARRIBA : Movimiento::ABAJO;

	int movimientoZ = 0;
	if (abs(dz) > deadZone)
		movimientoZ = dz > 0 ? Movimiento::ADELANTE : Movimiento::ATRAS;

	if (movimientoX == 0 && movimientoY == 0 && movimientoZ == 0) return;

	movimiento->setDireccionX(movimientoX);
	movimiento->setDireccionY(movimientoY);
	movimiento->setDireccionZ(movimientoZ);

	if (*movimiento == *ultimoMovimiento) return;

	movimiento->setX(x);
	movimiento->setY(y);
	movimiento->setZ(z);
	std::time_t tiempo;
	time(&tiempo);
	movimiento->setTiempo(tiempo);

	if (movimiento->getDireccionX() != Movimiento::SIN_DIRECCION || 
		movimiento->getDireccionY() != Movimiento::SIN_DIRECCION) {
		for (unsigned int i = 0; i < listeners.size(); i++)
			listeners.at(i)->updateMovimiento(movimiento);
		ultimoMovimiento = movimiento;
	}
}

Movimiento * ReconocedorBasico::lastMove() {
	return ultimoMovimiento;
}

void ReconocedorBasico::addListener(ListenerMovimiento * listener) {
	listeners.push_back(listener);
}