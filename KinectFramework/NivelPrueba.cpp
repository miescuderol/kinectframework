#include "NivelPrueba.h"
#include <iostream>


NivelPrueba::NivelPrueba(Sensor * kinect) {
	this->kinect = kinect;
	ciclos = 0;
	jugadorActivo = 100000000;
}


NivelPrueba::~NivelPrueba(void) {

}

void NivelPrueba::cargar(Escena * nivelAnt) {
	std::cout << "Cargando NivelPrueba" << std::endl;
	float i = 0;
	kinect->addListenerJugadorNuevo(this);
	while (i < 30000) i++;
}

bool NivelPrueba::isTerminada() {
	return (ciclos > 5000);
}

void NivelPrueba::update() {
	ciclos++;
}

void NivelPrueba::updateJugadorNuevo(JugadorID user) {
	jugadorActivo = user;
	std::cout << "jugador encontrado en el nivel " << user << std::endl;
}

Estado NivelPrueba::getEstadoFinal() {
	return 1;
}

void NivelPrueba::descargar()
{
	throw std::exception("The method or operation is not implemented.");
}
