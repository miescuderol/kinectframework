#include "NivelPrueba.h"
#include <iostream>


NivelPrueba::NivelPrueba(Kinect * kinect) {
	this->kinect = kinect;
	ciclos = 0;
}


NivelPrueba::~NivelPrueba(void) {

}

void NivelPrueba::cargar(Nivel * nivelAnt) {
	std::cout << "Cargando NivelPrueba" << std::endl;
	float i = 0;
	while (i < 30000) i++;
}

bool NivelPrueba::isTerminado() {
	return (ciclos > 500000);
}

void NivelPrueba::update() {
	ciclos++;
	std::cout << "ID Activo: " << kinect->getIDActivo() << " ciclos: " << ciclos << std::endl;
}

Estado NivelPrueba::getEstadoFinal() {
	return Estado::SALIDA_1;
}
