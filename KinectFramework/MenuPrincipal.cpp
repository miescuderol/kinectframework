#include "MenuPrincipal.h"
#include "Kinect.h"

MenuPrincipal::MenuPrincipal(Sensor * kinect, Rendering * rendering) {
	this->kinect = kinect;
	this->rendering = rendering;
	idMano = -1;
	punto = NULL;
	estadoSiguiente = NINGUNO;
}

MenuPrincipal::~MenuPrincipal(void) {
}

void MenuPrincipal::cargar( Escena * escenaAnt ) {
	kinect->addListenerJugadorNuevo(this);
	kinect->addListenerJugadorPerdido(this);

	rendering->setEscena(this);

	texturaFondo = rendering->LoadTexture("data/images/menuEscrito.tga");
	texturaClick = rendering->LoadTexture("data/images/mano.png");
}

bool MenuPrincipal::isTerminada() {
	if (estadoSiguiente != NINGUNO)
		return true;
	return false;
}

void MenuPrincipal::update() {

	const Esqueleto * esq = kinect->getArticulaciones(idMano);
	int x = -1;
	int y = -1;
	if (esq != NULL) {
		const Articulacion * art = esq->getArticulacion(NiTEEsqueleto::MANO_DER);
		punto = art->getPosicion();
		x = punto->X();
		y = punto->Y();
	}

	if (0.55 > x && x > 0.45 && 0.41 < y && y < 0.44) {
		clickNeeded = true;
		estadoSiguiente = JUEGO_NUEVO;
	} else if (0.55 > x && x > 0.45 && 0.57 < y && y < 0.62) {
		clickNeeded = true;
		estadoSiguiente = SALIR;
	} else if (0.6 > x && x > 0.4 && 0.8 < y && y < 0.85) {
		clickNeeded = true;
		estadoSiguiente = CREDITS;
	} else {
		clickNeeded = false;
		estadoSiguiente = NINGUNO;
	}
	//estadoSiguiente = JUEGO_NUEVO;

}

Estado MenuPrincipal::getEstadoFinal() {
	return estadoSiguiente;
}

void MenuPrincipal::descargar() {
	kinect->removeListenerJugadorNuevo(this);
	kinect->removeListenerJugadorPerdido(this);
}

void MenuPrincipal::renderizar() {
	system("cls");
	std::cout << "MENU PRINCIPAL" << std::endl;

	if (punto != NULL)
		std::cout << punto->X() << "," << punto->Y() << "," << punto->Z() << std::endl;
	else
		std::cout << "Mano no detectada." << std::endl;

	glDisable(GL_LIGHTING);
	glClear( GL_COLOR_BUFFER_BIT );
	glEnable(GL_TEXTURE_2D); //Enable texture

	glBindTexture(GL_TEXTURE_2D,texturaFondo);//Binding texture
	glBegin(GL_QUADS);

	glColor4f(1.0,1.0,1.0,1.0);
	glTexCoord2f(0.0, 1.0); glVertex2i(-1.0, 1.0);
	glTexCoord2f(0.0, 0.0); glVertex2i(-1.0, -1.0);
	glTexCoord2f(1.0, 0.0); glVertex2i(1.0, -1.0);
	glTexCoord2f(1.0, 1.0); glVertex2i(1.0, 1.0);

	glEnd();
	glDisable(GL_TEXTURE_2D);

	if (clickNeeded)
		drawClick();

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
}

void MenuPrincipal::updateJugadorNuevo( JugadorID user ) {
	idMano = user;
}

void MenuPrincipal::updateJugadorPerdido( JugadorID user ) {
	idMano = -1;
}

void MenuPrincipal::drawClick() {
	glAlphaFunc(GL_GREATER, 0.5);

	glDisable(GL_LIGHTING);
	glEnable(GL_ALPHA_TEST);

	glEnable(GL_TEXTURE_2D); //Enable texture

	glBindTexture(GL_TEXTURE_2D,texturaClick);//Binding texture
	
	glBegin(GL_QUADS);
	glColor4f(1.0,1.0,1.0,1.0);
	glTexCoord2f(0.0, 0.0); glVertex2i(0.88, 0.88);
	glTexCoord2f(0.0, 1.0); glVertex2i(0.88, 0.95);
	glTexCoord2f(1.0, 1.0); glVertex2i(0.95, 0.95);
	glTexCoord2f(1.0, 0.0); glVertex2i(0.95, 0.88);

	glEnd();

	glDisable(GL_TEXTURE_2D);

	glDisable(GL_ALPHA_TEST);
	glEnable(GL_LIGHTING);
}
