#pragma once
#include "subsistema.h"
#include "Renderizable.h"
class Rendering
{
public:
	Rendering(void);
	~Rendering(void);

	void setup();

	bool update();

	void shutdown();

	void setEscena(Renderizable * escena);

private:
	int estadoOK;
	Renderizable * escena;

};

