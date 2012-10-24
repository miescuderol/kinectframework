#include "NivelPrueba2.h"
#include <iostream>

NivelPrueba2::NivelPrueba2(Kinect * kinect) {
	this->kinect = kinect;
	ciclos = 0;
}


NivelPrueba2::~NivelPrueba2(void) {

}

void NivelPrueba2::cargar(Nivel * nivelAnt) {
	std::cout << std::endl << std::endl << "Cargando NivelPrueba2" << std::endl;
	float i = 0;
	while (i < 30000) i++;
}

bool NivelPrueba2::isTerminado() {
	return (ciclos > 500000);
}

void NivelPrueba2::update() {
	ciclos++;
	std::cout << "ID Activo: " << jugadorActivo << " ciclos: " << ciclos << std::endl;
}

void NivelPrueba2::update( XnUserID user ){
	jugadorActivo = user;
	std::cout << "jugador encontrado en el nivel " << user << std::endl;
}

Estado NivelPrueba2::getEstadoFinal() {
	return Estado::MUERTO;
}
