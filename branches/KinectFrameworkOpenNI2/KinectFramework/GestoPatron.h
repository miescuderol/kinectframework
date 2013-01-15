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
	const int length();

	bool operator==(GestoPatron & otroGesto);
	bool operator!=(GestoPatron & otroGesto);

private:

	std::vector<Movimiento> movimientos;

};

