#include "Rendering.h"
#include <stdlib.h>
#include <sstream>
#include <iostream>

Rendering::Rendering(void) {
}

Rendering::~Rendering(void) {
}

void Rendering::setup() {
	tituloVentana = "KinectFramework";
	std::cout << "Rendering::setup Inicializando el subsistema de rendering...";

	estadoOK = glfwInit();
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT);
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f ); //clear background screen to black


	width = 512;
	height = 512;
	estadoOK = glfwOpenWindow( width, height, 0,0,0,0,0,0, GLFW_WINDOW );
	glfwSetWindowTitle(tituloVentana.c_str());


	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, width/height, 00.0, width);
	glMatrixMode(GL_MODELVIEW);
	if (!estadoOK)
		glfwTerminate();
	std::cout << " inicializado." << std::endl;
}

bool Rendering::update() {
	if (estadoOK) {

		//Clear information from last draw
		glClear( GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
		escena->renderizar();
		// Swap front and back rendering buffers
		glfwSwapBuffers();
// 		std::cout << calcFPS() << std::endl;              // Actualiza en consola los FPS una vez por segundo
 		calcFPS(0.5);           // Actualiza en consola los FPS cada dos segundos
// 		calcFPS(1.0, tituloVentana);              // Actualiza los FPS en el titulo de la ventana una vez por segundo
// 		calcFPS(2.0, tituloVentana);              // Actualiza los FPS en el titulo de la ventana una vez cada dos segundos
// 		calcFPS(3.0, "Current FPS: ");          // Escribe en el titulo de la ventana "FPS: " + FPS cada 3 segundos
		return true;
	} else {
		return false;
	}
}

void Rendering::shutdown() {
	std::cout << "Rendering::shutdown Cerrando el subsistema de rendering...";
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

GLuint Rendering::LoadTexture(const char* TextureName) {

	GLuint Texture;  //variable for texture

	glGenTextures(1,&Texture); //allocate the memory for texture

	glBindTexture(GL_TEXTURE_2D,Texture); //Binding the texture


	if(glfwLoadTexture2D(TextureName, GLFW_BUILD_MIPMAPS_BIT)){

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

		return Texture;

	}else return -1;

}

int Rendering::getXRes()
{
	return width;
}

int Rendering::getYRes()
{
	return height;
}

double Rendering::calcFPS(double theTimeInterval, std::string theWindowTitle) {
	// Static values which only get initialised the first time the function runs
	static double t0Value       = glfwGetTime(); // Set the initial time to now
	static int    fpsFrameCount = 0;             // Set the initial FPS frame count to 0
	static double fps           = 0.0;           // Set the initial FPS value to 0.0

	// Get the current time in seconds since the program started (non-static, so executed every time)
	double currentTime = glfwGetTime();

	// Ensure the time interval between FPS checks is sane (low cap = 0.1s, high-cap = 10.0s)
	// Negative numbers are invalid, 10 fps checks per second at most, 1 every 10 secs at least.
	if (theTimeInterval < 0.1)
	{
		theTimeInterval = 0.1;
	}
	if (theTimeInterval > 10.0)
	{
		theTimeInterval = 10.0;
	}

	// Calculate and display the FPS every specified time interval
	if ((currentTime - t0Value) > theTimeInterval)
	{
		// Calculate the FPS as the number of frames divided by the interval in seconds
		fps = (double)fpsFrameCount / (currentTime - t0Value);

		// If the user specified a window title to append the FPS value to...
		if (theWindowTitle != "NONE")
		{
			// Convert the fps value into a string using an output stringstream
			std::ostringstream stream;
 			stream << fps;
			std::string fpsString = stream.str();

			// Append the FPS value to the window title details
			theWindowTitle += " | FPS: " + fpsString;

			// Convert the new window title to a c_str and set it
			const char* pszConstString = theWindowTitle.c_str();
			glfwSetWindowTitle(pszConstString);
		}
		else // If the user didn't specify a window to append the FPS to then output the FPS to the console
		{
			std::cout << "FPS: " << fps << std::endl;
		}

		// Reset the FPS frame counter and set the initial time to be now
		fpsFrameCount = 0;
		t0Value = glfwGetTime();
	}
	else // FPS calculation time interval hasn't elapsed yet? Simply increment the FPS frame counter
	{
		fpsFrameCount++;
	}

	// Return the current FPS - doesn't have to be used if you don't want it!
	return fps;
}