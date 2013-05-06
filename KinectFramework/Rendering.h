#pragma once
#include "subsistema.h"
#include "Renderizable.h"
#include <GL/glfw.h>
#include <string>
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

	GLuint LoadTexture(const char* TextureName);

	int getXRes();

	int getYRes();

	double calcFPS(double theTimeInterval = 1.0, std::string theWindowTitle = "NONE");

private:
	int estadoOK;
	Renderizable * escena;
	int width, height;

	std::string tituloVentana;
};

