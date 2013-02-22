#include "NivelPrueba2.h"
#include <iostream>

NivelPrueba2::NivelPrueba2(Sensor * kinect) {
	this->kinect = kinect;
	ciclos = 0;
}


NivelPrueba2::~NivelPrueba2(void) {

}

void NivelPrueba2::cargar(Escena * nivelAnt) {
	std::cout << std::endl << std::endl << "Cargando NivelPrueba2" << std::endl;
	float i = 0;
	while (i < 30000) i++;
}

bool NivelPrueba2::isTerminada() {
	return (ciclos > 500000);
}

void NivelPrueba2::update() {
	ciclos++;
}

void NivelPrueba2::updateJugadorNuevo(JugadorID user){
	jugadorActivo = user;
	std::cout << "jugador encontrado en el nivel " << user << std::endl;
}

Estado NivelPrueba2::getEstadoFinal() {
	return 0;
}

void NivelPrueba2::descargar()
{
	throw std::exception("The method or operation is not implemented.");
}
