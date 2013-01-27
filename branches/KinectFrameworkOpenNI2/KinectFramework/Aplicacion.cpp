#include "Aplicacion.h"
#include <iostream>

Aplicacion::Aplicacion() {

}


void Aplicacion::setup() {
	//PRE: grafoNiveles != NULL
	
	nivelActivo = grafoNiveles->getInicio();

	for (unsigned int i = 0; i < subsistemasPreNivel.size(); i++) {
		subsistemasPreNivel[i]->setup();
	}
	nivelActivo->cargar(NULL);
	for (unsigned int i = 0; i < subsistemasPostNivel.size(); i++) {
		subsistemasPostNivel[i]->setup();
	}

}

void Aplicacion::update() {

	//Hace el cambio de nivel si es necesario
	if (nivelActivo->isTerminado()) {
		Escena * nivelAnt = nivelActivo;
		nivelActivo = grafoNiveles->getSigEscena(nivelActivo, nivelActivo->getEstadoFinal());
		nivelActivo->cargar(nivelAnt);

	}

	for (unsigned int i = 0; i < subsistemasPreNivel.size(); i++) {
		subsistemasPreNivel[i]->update();
	}
	nivelActivo->update();
	for (unsigned int i = 0; i < subsistemasPostNivel.size(); i++) {
		subsistemasPostNivel[i]->update();
	}

}


void Aplicacion::exit() {
	if (sensor != NULL){
		std::cout << "Cerrando aplicacion." << std::endl;
		sensor->shutdown();
		sensor->~Sensor();
	}
}

void Aplicacion::setGrafoJuego(GrafoEscenas * grafo) {
	grafoNiveles = grafo;
}

void Aplicacion::run() {

	if (grafoNiveles == NULL) return;

	std::cout << "Iniciando setup... ";
	initComponentes();
	setup();
	std::cout << "hecho." << std::endl;
	if(nivelActivo == NULL) std::cout << "nivel activo es null" << std::endl;
	std::cout << "Iniciando Game Loop... " << std::endl;
	while (!grafoNiveles->isFinal(nivelActivo) || !nivelActivo->isTerminado()) {
		update();
		draw();
	}
	exit();

}

void Aplicacion::addSubsistemaPreNivel(Subsistema * subsistema) {
	subsistemasPreNivel.push_back(subsistema);
}

void Aplicacion::addSubsistemaPostNivel(Subsistema * subsistema) {
	subsistemasPostNivel.push_back(subsistema);
}

Sensor * Aplicacion::getSensor() {
	return sensor;
}

void Aplicacion::setSensor( Sensor * sensor ) {
	this->sensor = sensor;
}
