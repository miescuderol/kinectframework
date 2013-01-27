#pragma once
#include <map>
#include "Escena.h"

class GrafoEscenas
{

public:

	GrafoEscenas(void);
	~GrafoEscenas(void);

	void addEscena(Escena * n);
	void addArco(Escena * origen, Estado e, Escena * destino);
	bool setInicio(Escena * n);
	bool setFinal(Escena * n);

	Escena * getSigEscena(Escena * n, Estado e);
	Escena * getInicio();

	bool isFinal(Escena * n);

private:

	Escena * inicio, * ultimo;
	std::map<Escena *, std::map<Estado, Escena *>> grafo;

};

