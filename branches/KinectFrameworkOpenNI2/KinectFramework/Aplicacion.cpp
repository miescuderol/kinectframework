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
		escenaAnt->descargar();
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
	std::cout << "exit()... " << std::endl;
	for (int i = subsistemasPostEscena.size() - 1; i >= 0 ; i--) {
		std::cout << "cerrando subsistemas post escena" << std::endl;
		subsistemasPostEscena[i]->shutdown();
	}
	
	for (int i = subsistemasPreEscena.size() - 1; i >= 0 ; i--) {
		std::cout << "cerrando subsistemas pre escena" << std::endl;
		subsistemasPreEscena[i]->shutdown();
	}
	

	if (sensor != NULL){
		std::cout << "Cerrando aplicacion." << std::endl;
		sensor->shutdown();
		sensor->~Sensor();
	}
	std::cout << "Aplicacion cerrada, presione una tecla para continuar" << std::endl;
	char tecla;
	std::cin >> tecla;
}

void Aplicacion::setGrafoJuego(GrafoEscenas * grafo) {
	grafoEscenas = grafo;
}

void Aplicacion::run() {

	if (grafoEscenas == NULL) return;

	std::cout << "Iniciando setup... " << std::endl;
	if(initComponentes()) {
		std::cout << "componentes inicializados" << std::endl;
			
		setup();
		std::cout << "hecho." << std::endl;
		if(escenaActiva == NULL) std::cout << "escena activa es null" << std::endl;
		std::cout << "Iniciando Game Loop... " << std::endl;
		while (!grafoEscenas->isFinal(escenaActiva) || !escenaActiva->isTerminada()) {
			update();
			draw();
		}
	} else
		std::cout << "error en initComponentes() " << std::endl;
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
