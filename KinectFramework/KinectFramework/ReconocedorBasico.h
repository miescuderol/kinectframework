#pragma once

#include <vector>
//#include "ListenerDirecciones.h"

class ListenerDirecciones; // forward declaration por referencia cruzada

typedef enum Direccion {SIN_DIRECCION,a,b,c};

typedef struct Movimiento { // capaz que hay que hacerlo clase y hacer el operador ==
	Direccion vertical, horizontal;
};


class ReconocedorBasico
{

public:

	ReconocedorBasico(int divisions, float deadZone);
	~ReconocedorBasico(void);

	void setNewPosition(float x, float y, float z);
	Movimiento lastMove();
	void addListener(ListenerDirecciones * listener);

	
private:

	Movimiento ultimoMovimiento;
	std::vector<ListenerDirecciones*> listeners;
	float prevX, prevY, prevZ;
	float deadZone;
	int divisions;
	float amplitude;
	float displacement;

};

