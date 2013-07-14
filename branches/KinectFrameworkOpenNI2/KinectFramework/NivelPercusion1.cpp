#include "NivelPercusion1.h"


NivelPercusion1::NivelPercusion1(Sensor * kinect, Rendering * rendering) {
	this->kinect = kinect;
	this->rendering = rendering;
	ancho = rendering->getXRes();
	alto = rendering->getYRes();
}

NivelPercusion1::~NivelPercusion1() {
}

void NivelPercusion1::cargar( Escena * escenaAnt ) {

	kinect->enableGenerator(Sensor::USER_GENERATOR);

	kinect->addListenerJugadorNuevo(this);
	kinect->addListenerJugadorPerdido(this);
	kinect->addListenerJugadorCalibrado(this);

	std::vector<JugadorID> jugadoresActivos = kinect->getJugadoresActivos();
	for (int i = 0; i < jugadoresActivos.size(); i++)
		updateJugadorCalibrado(jugadoresActivos[i]);

	ganador = -1;

	Movimiento arriba;
	arriba.setDireccionX(Movimiento::NO_IMPORTA);
	arriba.setDireccionY(Movimiento::ARRIBA);
	arriba.setDireccionZ(Movimiento::NO_IMPORTA);

	Movimiento abajo;
	abajo.setDireccionX(Movimiento::NO_IMPORTA);
	abajo.setDireccionY(Movimiento::ABAJO);
	abajo.setDireccionZ(Movimiento::NO_IMPORTA);

	gestoGolpe = new GestoPatron("GOLPE");
	gestoGolpe->addMovement(arriba);
	gestoGolpe->addMovement(abajo);

	pulso = 0.5;
	errorMargin = 0.02;

	lastTick = glfwGetTime();

	delay = pulso;

	rendering->setEscena(this);

	texturaBongo = rendering->LoadTexture("data/images/bongo.tga");
	glBindTexture(GL_TEXTURE_2D,texturaBongo);
}

bool NivelPercusion1::isTerminada() {
	int max = 0, iMax = 0, segundo = 0;
	for (int i = 0; i < puntos.size(); i++) {
		int puntosI = puntos[i];
		if (puntosI > max) {
			max = puntosI;
			iMax = i;
		}
		if (puntosI < max && puntosI > segundo) {
			segundo = puntosI;
		}
	}
	if (max+15 > segundo) {
		ganador = iMax;
		return false;
	}
	return false;
}

void NivelPercusion1::update() {
	double now = glfwGetTime();
	std::cout << "now: " << now << std::endl;
	if ((pulso < now - lastTick)&&(now - lastTick < pulso+errorMargin)) {
		mostrarBoton = true;
		lastTick = now;
	} else {
		mostrarBoton = false;
	}
	if (pulso+errorMargin < now - lastTick)
		lastTick = now;
}

Estado NivelPercusion1::getEstadoFinal() {
	return 1;
}

void NivelPercusion1::descargar() {
	kinect->removeListenerJugadorCalibrado(this);
	kinect->removeListenerJugadorPerdido(this);
	kinect->removeListenerJugadorNuevo(this);
}

void NivelPercusion1::updateJugadorNuevo( JugadorID user ) {
	
}

void NivelPercusion1::updateJugadorPerdido( JugadorID user ) {
	int i = 0;
	// Borra el jugador
	for (std::vector<JugadorID>::iterator it = jugadores.begin(); it != jugadores.end();) {
		if (*it == user) {
			jugadores.erase(it);
			break;
		}
		i++;
	}

	// Borra los puntos asociados al jugador
	std::vector<JugadorID>::iterator it = puntos.begin();
	for (int j = 0; j < i; j++) {
		it++;
	}
	puntos.erase(it);
}

void NivelPercusion1::updateJugadorCalibrado( JugadorID user ) {
	jugadores.push_back(user);
	puntos.push_back(0);

	idReconocedores.push_back(kinect->startReconocedor(user, NiTEEsqueleto::MANO_DER, gestoGolpe));
	idReconocedores.push_back(kinect->startReconocedor(user, NiTEEsqueleto::MANO_IZQ, gestoGolpe));
}

void NivelPercusion1::renderizar() {
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_QUADS);
	glVertex3f(-1.0, 1.0, -1.50);
	glVertex3f(1.0, 1.0, -1.50);
	glVertex3f(1.0, -1.0, -1.50);
	glVertex3f(-1.0, -1.0, -1.50);
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);    glVertex3f(-0.8, -0.4, -1.49);
	glTexCoord2f(1, 0);    glVertex3f(-0.2, -0.4, -1.49);
	glTexCoord2f(1, 1);    glVertex3f(-0.2, -0.1, -1.49);
	glTexCoord2f(0, 1);    glVertex3f(-0.8, -0.1, -1.49);
	glEnd();
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);

	if(mostrarBoton){
		std::cout << "rendering" << std::endl;
		glColor3f(0.0, 0.8, 0.2);
		glBegin(GL_QUADS);
		glVertex3f(-0.2, 0.8, -1.49);
		glVertex3f(0.2, 0.8, -1.49);
		glVertex3f(0.2, 0.7, -1.49);
		glVertex3f(-0.2, 0.7, -1.49);
		glEnd();
	}
}

void NivelPercusion1::updateGesto( Gesto * m ) {
	JugadorID id = atoi(m->getArticulacion().substr(0, m->getArticulacion().find_first_of('_')).data());
	for (int i = 0; i < jugadores.size(); i++) {
		if (jugadores[i] = id)
			if (golpeValido())
				puntos[i]++;
			else puntos[i]--;
	}
}

bool NivelPercusion1::golpeValido() {
	double now = glfwGetTime();

	if ((now - lastTick > delay-errorMargin)&&(now - lastTick < delay+errorMargin)) {
		return true;
	}
	return false;
}
