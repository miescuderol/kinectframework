#pragma once
#include <vector>
#include "Nivel.h"

class Grafo
{

public:

	Grafo(void);
	~Grafo(void);

	void addNivel(Nivel * n);
	void addArco(Nivel * origen, Estado * e, Nivel * destino);
	Nivel * getSigNivel(Nivel * n, Estado * e);
	Nivel * getInicio();

private:

	std::vector<Nivel *> estados;
	std::vector<Estado *> arcos;

};

