#include "Aplicacion.h"
#include <iostream>

Aplicacion::Aplicacion() {

}


void Aplicacion::setup() {
	//PRE: grafoEscenas != NULL
	
	escenaActiva = grafoEscenas->getInicio();

	for (unsigned int i = 0; i < subsistemasPreEscena.size(); i++) {
		subsistemasPreEscena[i]->setup();
	}
	escenaActiva->cargar(NULL);
	for (unsigned int i = 0; i < subsistemasPostEscena.size(); i++) {
		subsistemasPostEscena[i]->setup();
	}

}

void Aplicacion::update() {

	//Hace el cambio de escena si es necesario
	if (escenaActiva->isTerminada()) {
		Escena * escenaAnt = escenaActiva;
		escenaActiva = grafoEscenas->getSigEscena(escenaActiva, escenaActiva->getEstadoFinal());
		escenaActiva->cargar(escenaAnt);

	}

	for (unsigned int i = 0; i < subsistemasPreEscena.size(); i++) {
		subsistemasPreEscena[i]->update();
	}
	escenaActiva->update();
	for (unsigned int i = 0; i < subsistemasPostEscena.size(); i++) {
		subsistemasPostEscena[i]->update();
	}

}


void Aplicacion::exit() {
	for (unsigned int i = subsistemasPostEscena.size() - 1; i >= 0 ; i--) 
		subsistemasPostEscena[i]->shutdown();

	for (unsigned int i = subsistemasPreEscena.size() - 1; i >= 0 ; i--) 
		subsistemasPreEscena[i]->shutdown();
	

	if (sensor != NULL){
		std::cout << "Cerrando aplicacion." << std::endl;
		sensor->shutdown();
		sensor->~Sensor();
	}
}

void Aplicacion::setGrafoJuego(GrafoEscenas * grafo) {
	grafoEscenas = grafo;
}

void Aplicacion::run() {

	if (grafoEscenas == NULL) return;

	std::cout << "Iniciando setup... ";
	initComponentes();
	setup();
	std::cout << "hecho." << std::endl;
	if(escenaActiva == NULL) std::cout << "escena activa es null" << std::endl;
	std::cout << "Iniciando Game Loop... " << std::endl;
	while (!grafoEscenas->isFinal(escenaActiva) || !escenaActiva->isTerminada()) {
		update();
		draw();
	}
	exit();

}

void Aplicacion::addSubsistemaPreEscena(Subsistema * subsistema) {
	subsistemasPreEscena.push_back(subsistema);
}

void Aplicacion::addSubsistemaPostEscena(Subsistema * subsistema) {
	subsistemasPostEscena.push_back(subsistema);
}

Sensor * Aplicacion::getSensor() {
	return sensor;
}

void Aplicacion::setSensor( Sensor * sensor ) {
	this->sensor = sensor;
}
