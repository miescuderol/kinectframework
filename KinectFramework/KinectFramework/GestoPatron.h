#pragma once
#include <vector>
#include "ReconocedorBasico.h"

class GestoPatron
{
public:
	GestoPatron(void);
	~GestoPatron(void);
	void addMovement(Movimiento m);
	void insertAt(int i, Movimiento m);
	Movimiento * getMovement(int i);
	int length();

private:

	std::vector<Movimiento> movimientos;

};

