#pragma once

#include <vector>
#include "Movimiento.h"
//#include "ListenerDirecciones.h"

class ListenerMovimiento; // forward declaration por referencia cruzada

class ReconocedorBasico
{

public:

	ReconocedorBasico(int divisions, float deadZone);
	~ReconocedorBasico(void);

	void setNewPosition(float x, float y, float z);
	Movimiento * lastMove();
	void addListener(ListenerMovimiento * listener);

	
private:

	Movimiento * ultimoMovimiento;
	std::vector<ListenerMovimiento*> listeners;
	float prevX, prevY, prevZ;
	float deadZone;
	int divisions;
	float amplitude;
	float displacement;

	Movimiento::Direccion intADireccion(int i);

};

