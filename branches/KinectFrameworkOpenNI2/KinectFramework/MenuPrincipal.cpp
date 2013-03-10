#include "MenuPrincipal.h"
#include <GL/glfw.h>


MenuPrincipal::MenuPrincipal(Sensor * kinect, Rendering * rendering) {
	this->kinect = kinect;
	this->rendering = rendering;
	idMano = -1;
	punto = NULL;
}

MenuPrincipal::~MenuPrincipal(void) {
}

void MenuPrincipal::cargar( Escena * escenaAnt ) {
	kinect->addListenerManoNueva(this);
	kinect->addListenerManoPerdida(this);

	rendering->setEscena(this);
}

bool MenuPrincipal::isTerminada() {
	if (punto != NULL && punto->X() > 640*0.75 && punto->Y() > 480*0.75)
		return true;
	return false;
}

void MenuPrincipal::update() {

	system("cls");
	std::cout << "MENU PRINCIPAL" << std::endl;

	punto = kinect->getMano(idMano);
	if (punto != NULL)
		std::cout << punto->X() << "," << punto->Y() << "," << punto->Z() << std::endl;
	else
		std::cout << "Mano no detectada." << std::endl;

}

Estado MenuPrincipal::getEstadoFinal() {
	return 1;
}

void MenuPrincipal::descargar() {
	kinect->removeListenerManoNueva(this);
	kinect->removeListenerManoPerdida(this);
}

void MenuPrincipal::updateManoNueva( JugadorID mano ) {
	idMano = mano;
}

void MenuPrincipal::updateManoPerdida( JugadorID mano ) {
	idMano = -1;
}

void MenuPrincipal::renderizar()
{
	glClear( GL_COLOR_BUFFER_BIT );
	glColor3f(0.0, 1.0, 1.0);
	glBegin(GL_QUADS);
	glVertex2f(0.5, 0.5);
	glVertex2f(0.5, 1);
	glVertex2f(1, 1);
	glVertex2f(1, 0.5);
	glEnd();

	glColor3f(1.0, 1.0, 1.0);
	if (punto != NULL) {
		glPointSize((20-punto->Z()/120) < 1? 1 : 20-punto->Z()/120);
		glBegin(GL_POINTS);
		float x = rendering->screenToWorldX(punto->X()+320);
		float y = rendering->screenToWorldY(punto->Y()+240);
		std::cout << "(" << x << "," << y << ")" << std::endl;
			glVertex3f(x, y, 0);
		glEnd();
	} else {
		glPointSize(10.0);
		glBegin(GL_POINTS);
			glVertex3f(0.0, 0.0, 0.0);
		glEnd();		
	}
	// Swap front and back rendering buffers
	glfwSwapBuffers();
}
