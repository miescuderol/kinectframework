#include "NivelPrueba.h"
#include "MiAplicacion.h"
#include <GL/glfw.h>
#include <iostream>


float profP = -80.0f;
float camX = 0.0f;
float camY = 0.0f;
float centroX = 0.0f;
float centroY = 0.0f;
float p = -3.0f;
NivelPrueba::NivelPrueba(Sensor * kinect, Rendering * rendering) {
	this->kinect = kinect;
	this->rendering = rendering;
	this->alto = rendering->getYRes();
	this->ancho = rendering->getXRes();
}


NivelPrueba::~NivelPrueba(void) {

}

void NivelPrueba::cargar(Escena * nivelAnt) {
	ciclos = 0;
	jugadorActivo = 100000000;
	jugadorCalibrado = false;
	kinect->enableGenerator(Sensor::USER_GENERATOR);
	std::cout << "NivelPrueba::cargar Cargando NivelPrueba" << std::endl;
	float i = 0;
	kinect->addListenerJugadorNuevo(this);
	kinect->addListenerJugadorPerdido(this);
	kinect->addListenerJugadorCalibrado(this);
	std::vector<JugadorID> jugadoresActivos = kinect->getJugadoresActivos();
	for (int i = 0; i < jugadoresActivos.size(); i++)
		updateJugadorCalibrado(jugadoresActivos[i]);
	while (i < 30000) i++;
	rendering->setEscena(this);
	cargarGL();
}

bool NivelPrueba::isTerminada() {
	return mundo->terminado();
}

void NivelPrueba::update() {
	ciclos++;
	for (int i = 0; i < jugadores.size(); i++) {
		const Esqueleto * esq = kinect->getArticulaciones(jugadores[i]);
		if (esq == NULL) return;
		const Articulacion * art[3];
		art[0] = esq->getArticulacion(NiTEEsqueleto::CABEZA);
		art[1] = esq->getArticulacion(NiTEEsqueleto::MANO_DER);
		art[2] = esq->getArticulacion(NiTEEsqueleto::MANO_IZQ);
		if(art[1]->getConfianza() > 0.6){
			mundo->setArticulaciones(art, 3);
		}
	}
}

void NivelPrueba::updateJugadorNuevo(JugadorID user) {
	std::cout << "NivelPrueba::updateJugadorNuevo Jugador " << user << " encontrado en el nivel 1" << std::endl;
}

Estado NivelPrueba::getEstadoFinal() {
		return 1;
}

void NivelPrueba::descargar() {
	kinect->removeListenerJugadorCalibrado(this);
	kinect->removeListenerJugadorPerdido(this);
	kinect->removeListenerJugadorNuevo(this);
}

void NivelPrueba::updateJugadorPerdido( JugadorID user ) {
	std::cout << "NivelPrueba::updateJugadorPerdido updateJugadorPerdido: " << user << std::endl;
	for (std::vector<JugadorID>::iterator it = jugadores.begin(); it != jugadores.end();) {
		if (*it == user) {
			jugadores.erase(it);
			break;
		}
	}
}

void NivelPrueba::updateJugadorCalibrado( JugadorID user ) {
	if(!jugadorCalibrado){
		jugadorCalibrado = true;
		std::cout << "NivelPrueba::updateJugadorCalibrado Jugador " << user << " calibrado." << std::endl;
		jugadorActivo = user;
		jugadores.push_back(user);
		mundo->empezar();
		mundo->setParticulasVivas(100);
	}
}

void NivelPrueba::setWorld(World * mundo){
	this->mundo = mundo;
}

void NivelPrueba::renderizar(){
	mundo->renderizar();
}

void NivelPrueba::cargarGL(){
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 0, alto/2, 0.0, 0.0 };
	glShadeModel(GL_SMOOTH);

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHT0);
}
	
