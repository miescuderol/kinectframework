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

	float screenToWorldX(int x);

	float screenToWorldY(int y);

	void setWorldX(int xRes);

	void setWorldY(int yRes);

private:
	int estadoOK;
	Renderizable * escena;
	int width, height;

};

