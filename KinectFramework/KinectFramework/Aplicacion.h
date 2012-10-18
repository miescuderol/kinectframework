#pragma once
#include "Grafo.h"
#include "Subsistema.h"
#include "Nivel.h"
#include <vector>

class Aplicacion
{
public:
	Aplicacion(void);
	~Aplicacion(void);
	void setGrafoJuego(Grafo * grafo);
	void run();
	void addSubsistemaPreNivel(Subsistema * subsistema);
	void addSubsistemaPostNivel(Subsistema * subsistema);

private:

	void setup();
	void update();
	void draw();
	void exit();

	Grafo * grafoNiveles;
	Nivel * nivelActivo;
	std::vector<Subsistema*> subsistemasPreNivel, subsistemasPostNivel;
};

