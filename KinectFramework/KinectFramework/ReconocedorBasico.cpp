#include "ReconocedorBasico.h"
#include "ListenerDirecciones.h"
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
	Movimiento movimiento;
	movimiento.horizontal = SIN_DIRECCION;
	movimiento.vertical = SIN_DIRECCION;

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

	movimiento.vertical = (alpha-displacement)/amplitude;
	movimiento.horizontal = (beta-displacement)/amplitude;

	if (movimiento.horizontal != SIN_DIRECCION || movimiento.vertical != SIN_DIRECCION) {
		for (unsigned int i = 0; i < listeners.size(); i++)
			listeners.at(i)->update(movimiento);
		ultimoMovimiento = movimiento;
	}
}

ReconocedorBasico::Movimiento ReconocedorBasico::lastMove() {
	return ultimoMovimiento;
}

void ReconocedorBasico::addListener(ListenerDirecciones * listener) {
	listeners.push_back(listener);
}