#include "ReconocedorBasico.h"
#include "ListenerMovimiento.h"
#include "math.h"
#include <iostream>

#define PI 3.14159

ReconocedorBasico::ReconocedorBasico(int divisiones, float deadZone) {
	this->divisions = divisiones;
	this->deadZone = deadZone;
	gamma = (float) (2*PI)/divisions;
	displacement = (divisions/2-1)*gamma/2;
	prevX = prevY = prevZ = 0;
	ultimoMovimiento = NULL;
}

ReconocedorBasico::~ReconocedorBasico(void) {

}

void ReconocedorBasico::setNewPosition(float x, float y, float z) {

	//std::cout << "RB: (" << x <<", " << y << ", " << z << ")" << std::endl;

	Movimiento * movimiento = new Movimiento();

	float dx = x - prevX;
	float dy = y - prevY;
	float dz = z - prevZ;

	//std::cout << "dx:" << dx << " dy:" << dy << " dz:" << dz << std::endl;

	double moduloXZ = sqrt(pow(dx,2)+pow(dz,2));
	double moduloXY = sqrt(pow(dx,2)+pow(dy,2));
	double modulo = sqrt(pow(dx,2)+pow(dy,2)+pow(dz,2));

	prevX = x;
	prevY = y;
	prevZ = z;
			
	if (modulo < deadZone) {
		delete movimiento;
		return;
	}
	
	if (moduloXZ > deadZone) {
		double alpha = asin(dx/moduloXZ);

		if (alpha-displacement < 0) alpha += 2*PI;

		int movimientoHorizontal = floor((alpha-displacement)/(2*gamma));

		if (dx < 0 && movimientoHorizontal != 0) movimientoHorizontal = divisions - movimientoHorizontal;

		movimiento->setDireccionHorizontal(movimientoHorizontal);

		std::cout << "MH: " << movimientoHorizontal << "    ";
	}
	
	if (moduloXY > deadZone) {
		double beta = asin(dy/moduloXY);

		if (beta-displacement < 0) beta += 2*PI;
	
		int movimientoVertical = floor((beta-displacement)/gamma);

		if (dx < 0 && movimientoVertical != 0) movimientoVertical = divisions - movimientoVertical;

		movimiento->setDireccionVertical(movimientoVertical);

	}

	if (ultimoMovimiento != NULL && *movimiento == *ultimoMovimiento) return;

	if (movimiento->getDireccionHorizontal() != Movimiento::SIN_DIRECCION || 
		movimiento->getDireccionVertical() != Movimiento::SIN_DIRECCION) {
		for (unsigned int i = 0; i < listeners.size(); i++)
			listeners.at(i)->updateMovimiento(movimiento);
		ultimoMovimiento = movimiento;
		std::cout << "MV: " << movimiento->getDireccionVertical() << std::endl;
	}
}

Movimiento * ReconocedorBasico::lastMove() {
	return ultimoMovimiento;
}

void ReconocedorBasico::addListener(ListenerMovimiento * listener) {
	listeners.push_back(listener);
}