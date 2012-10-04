#pragma once
#include "Grafo.h"

class Aplicacion
{
public:
	Aplicacion(void);
	~Aplicacion(void);
	void setup();
	void update();
	void draw();
	void exit();
	void setGraphGame(Grafo graphgame);
	void run();
	void addSubsystem();
};

