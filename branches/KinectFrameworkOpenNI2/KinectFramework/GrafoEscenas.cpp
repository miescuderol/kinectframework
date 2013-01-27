#include "GrafoEscenas.h"

GrafoEscenas::GrafoEscenas(void) {
	inicio = NULL;
}

GrafoEscenas::~GrafoEscenas(void) {

}

void GrafoEscenas::addEscena(Escena * n) {
	grafo[n];
}

void GrafoEscenas::addArco(Escena * origen, Estado e, Escena * destino) {
	std::map<Escena*,std::map<Estado, Escena *>>::iterator it;

	it = grafo.find(origen); // busca que origen exista
	if (it != grafo.end()) {
		grafo[origen][e] = destino;
	}
}

bool GrafoEscenas::setInicio(Escena * n) {
	std::map<Escena*,std::map<Estado, Escena *>>::iterator it = grafo.find(n);

	if (it == grafo.end()) {
		return false;
	} else {
		inicio = n;
		return true;
	}

}

bool GrafoEscenas::setFinal(Escena * n) {
	std::map<Escena*,std::map<Estado, Escena *>>::iterator it = grafo.find(n);

	if (it == grafo.end()) {
		return false;
	} else {
		ultimo = n;
		return true;
	}

}

Escena * GrafoEscenas::getSigEscena(Escena * n, Estado e) {
	std::map<Escena*,std::map<Estado, Escena *>>::iterator it = grafo.find(n);

	if (it != grafo.end()) {
		return grafo[n][e];
	} else {
		return NULL;
	}

}

Escena * GrafoEscenas::getInicio() {
	return inicio;
}

bool GrafoEscenas::isFinal(Escena * n) {
	if (n == ultimo)
		return true;
	return false;
}