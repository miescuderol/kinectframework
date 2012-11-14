#include "ReconocedorBasico.h"
#include "ListenerMovimiento.h"
#include "math.h"

#define PI 3.14159

ReconocedorBasico::ReconocedorBasico(int divisiones, float deadZone) {
	this->divisions = divisiones;
	this->deadZone = deadZone;
	amplitude = (2*PI)/divisions;
	displacement = (divisions/2-1)*amplitude/2;
}

ReconocedorBasico::~ReconocedorBasico(void) {

}

void ReconocedorBasico::setNewPosition(float x, float y, float z) {
	Movimiento * movimiento = new Movimiento();
	movimiento->setDireccionHorizontal(Movimiento::Direccion::SIN_DIRECCION);
	movimiento->setDireccionVertical(Movimiento::Direccion::SIN_DIRECCION);

	float dx = x - prevX;
	float dy = y - prevY;
	float dz = z - prevZ;

	prevX = x;
	prevY = y;
	prevZ = z;

	double modulo = sqrt(pow(dx,2)+pow(dy,2)+pow(dz,2));
		
	if (modulo < deadZone) return;

	double alpha = asin(dy/modulo);
	double beta = asin(dz/modulo);

	if (alpha-displacement < 0) alpha += 2*PI;
	if (beta-displacement < 0) beta += 2*PI;

	movimiento->setDireccionVertical(intADireccion((int)((alpha-displacement)/amplitude)));
	movimiento->setDireccionHorizontal(intADireccion((int)((beta-displacement)/amplitude)));

	if (movimiento->getDireccionHorizontal() != Movimiento::Direccion::SIN_DIRECCION || 
		movimiento->getDireccionVertical() != Movimiento::Direccion::SIN_DIRECCION) {
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

Movimiento::Direccion ReconocedorBasico::intADireccion(int i) {
	return Movimiento::Direccion::SIN_DIRECCION;
}