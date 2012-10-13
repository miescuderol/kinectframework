#pragma once
#include <map>
#include "Nivel.h"

class Grafo
{

public:

	Grafo(void);
	~Grafo(void);

	void addNivel(Nivel * n);
	void addArco(Nivel * origen, Estado * e, Nivel * destino);
	bool setInicio(Nivel * n);

	Nivel * getSigNivel(Nivel * n, Estado * e);
	Nivel * getInicio();

private:

	Nivel * inicio;
	std::map<Nivel *, std::map<Estado *, Nivel *>> grafo;

};

