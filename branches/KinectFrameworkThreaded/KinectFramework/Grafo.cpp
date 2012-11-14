#include "Grafo.h"

Grafo::Grafo(void) {
	inicio = NULL;
}

Grafo::~Grafo(void) {

}

void Grafo::addNivel(Nivel * n) {
	grafo[n];
}

void Grafo::addArco(Nivel * origen, Estado e, Nivel * destino) {
	std::map<Nivel*,std::map<Estado, Nivel *>>::iterator it;

	it = grafo.find(origen); // busca que origen exista
	if (it != grafo.end()) {
		grafo[origen][e] = destino;
	}
}

bool Grafo::setInicio(Nivel * n) {
	std::map<Nivel*,std::map<Estado, Nivel *>>::iterator it = grafo.find(n);

	if (it == grafo.end()) {
		return false;
	} else {
		inicio = n;
		return true;
	}

}

bool Grafo::setFinal(Nivel * n) {
	std::map<Nivel*,std::map<Estado, Nivel *>>::iterator it = grafo.find(n);

	if (it == grafo.end()) {
		return false;
	} else {
		ultimo = n;
		return true;
	}

}

Nivel * Grafo::getSigNivel(Nivel * n, Estado e) {
	std::map<Nivel*,std::map<Estado, Nivel *>>::iterator it = grafo.find(n);

	if (it != grafo.end()) {
		return grafo[n][e];
	} else {
		return NULL;
	}

}

Nivel * Grafo::getInicio() {
	return inicio;
}

bool Grafo::isFinal(Nivel * n) {
	if (n == ultimo)
		return true;
	return false;
}