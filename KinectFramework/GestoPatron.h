#pragma once
#include <vector>
#include "ReconocedorBasico.h"

class GestoPatron
{
public:
	GestoPatron(std::string nombre = "GESTO_SIN_NOMBRE");
	~GestoPatron(void);
	void addMovement(Movimiento m);
	void insertAt(int i, Movimiento m);
	Movimiento * getMovement(int i);
	const int length();
	void setNombre(std::string nombre);
	const std::string getNombre() const;

	bool operator==(GestoPatron & otroGesto);
	bool operator!=(GestoPatron & otroGesto);

private:

	std::vector<Movimiento> movimientos;
	std::string nombre;

};

