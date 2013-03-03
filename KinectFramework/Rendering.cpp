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
	estadoOK = glfwOpenWindow( 300,300, 0,0,0,0,0,0, GLFW_WINDOW );
	if (!estadoOK)
		glfwTerminate();
	std::cout << " inicializado." << std::endl;
}

bool Rendering::update() {
	if (estadoOK) {
		// OpenGL rendering goes here...
		glClear( GL_COLOR_BUFFER_BIT );
		int r, g, b;
		escena->getColorFondo(r, g, b);
		std::cout << "      Rendering, RGB: " << r << "," << g << "," << b << std::endl;
		glColor3f(((float)r/255), ((float)g/255), ((float)b/255));
		glBegin(GL_QUADS); // Start drawing a quad primitive  

		glVertex3f(-1.0f, -1.0f, 0.0f); // The bottom left corner  
		glVertex3f(-1.0f, 1.0f, 0.0f); // The top left corner  
		glVertex3f(1.0f, 1.0f, 0.0f); // The top right corner  
		glVertex3f(1.0f, -1.0f, 0.0f); // The bottom right corner  

		glEnd();  
		// Swap front and back rendering buffers
		glfwSwapBuffers();
		// Check if ESC key was pressed or window was closed
		estadoOK = !glfwGetKey( GLFW_KEY_ESC ) &&
			glfwGetWindowParam( GLFW_OPENED );
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
