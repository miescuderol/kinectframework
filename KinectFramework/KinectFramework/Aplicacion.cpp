#include "Aplicacion.h"


Aplicacion::Aplicacion(void) {

}


Aplicacion::~Aplicacion(void) {

}

void Aplicacion::setup() {
	//PRE: grafoNiveles != NULL

	nivelActivo = grafoNiveles->getInicio();

	for (int i = 0; i < subsistemasPreNivel.size(); i++) {
		subsistemasPreNivel[i]->setup();
	}

	for (int i = 0; i < subsistemasPostNivel.size(); i++) {
		subsistemasPostNivel[i]->setup();
	}

}

void Aplicacion::update() {

	//Hace el cambio de nivel si es necesario
	if (nivelActivo->isTerminado()) {
		nivelActivo = grafoNiveles->getSigNivel(nivelActivo, nivelActivo->getEstadoFinal());
	}

	for (int i = 0; i < subsistemasPreNivel.size(); i++) {
		subsistemasPreNivel[i]->setup();
	}
	nivelActivo->update();
	for (int i = 0; i < subsistemasPostNivel.size(); i++) {
		subsistemasPostNivel[i]->setup();
	}

}

void Aplicacion::draw() {

}

void Aplicacion::exit() {

}

void Aplicacion::setGrafoJuego(Grafo * grafo) {
	grafoNiveles = grafo;
}

void Aplicacion::run() {

	if (grafoNiveles == NULL) return;

	setup();
	while (!grafoNiveles->isFinal(nivelActivo)) {
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