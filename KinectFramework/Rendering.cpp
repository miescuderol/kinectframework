#include "Rendering.h"
#include <GL/glfw.h>
#include <stdlib.h>
#include <iostream>

Rendering::Rendering(void) {
}

Rendering::~Rendering(void) {
}

void Rendering::setup() {
	std::cout << "Inicializando el subsistema de rendering...";
	estadoOK = glfwInit();
	width = 640;
	height = 480;
	estadoOK = glfwOpenWindow( width, height, 0,0,0,0,0,0, GLFW_WINDOW );
	glDisable(GL_LIGHTING);
	if (!estadoOK)
		glfwTerminate();
	std::cout << " inicializado." << std::endl;
}

bool Rendering::update() {
	if (estadoOK) {
		escena->renderizar();
		return true;
	} else {
		return false;
	}
}

void Rendering::shutdown() {
	std::cout << "Cerrando el subsistema de rendering...";
	glfwTerminate();
	std::cout << " cerrado." << std::endl;
}

void Rendering::setEscena( Renderizable * escena ) {
	this->escena = escena;
}

float Rendering::screenToWorldX( int x ) {
	return (float)(x-(width/2))/(float)width;
}

float Rendering::screenToWorldY( int y ) {
	return (float)(y-height/2)/(float)height;
}


