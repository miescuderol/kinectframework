#include "NivelPrueba.h"
#include <iostream>


NivelPrueba::NivelPrueba(Kinect * kinect) {
	this->kinect = kinect;
	ciclos = 0;
	jugadorActivo = 100000000;
}


NivelPrueba::~NivelPrueba(void) {

}

void NivelPrueba::cargar(Nivel * nivelAnt) {
	std::cout << "Cargando NivelPrueba" << std::endl;
	float i = 0;
	kinect->addListenerJugadorNuevo(this);
	while (i < 30000) i++;
}

bool NivelPrueba::isTerminado() {
	return (ciclos > 5000);
}

void NivelPrueba::update() {
	ciclos++;
}

void NivelPrueba::updateJugadorNuevo(XnUserID user) {
	jugadorActivo = user;
	std::cout << "jugador encontrado en el nivel " << user << std::endl;
}

Estado NivelPrueba::getEstadoFinal() {
	return Estado::SALIDA_1;
}
