#include "Reconocedor.h"
#include <iostream>
#include <fstream>

Reconocedor::Reconocedor(GestoPatron * gestoPatron, std::string idJugador_Art, ReconocedorBasico * recBasicos) {
	std::cout << "Reconocedor creado." << std::endl;
	this->recBasicos = recBasicos;
	this->idJugador_Art = idJugador_Art;
	this->gestoPatron = gestoPatron;
	ultimoMovimiento = 0;
	movimientoEsperado = gestoPatron->getMovement(ultimoMovimiento);
	this->recBasicos->addListener(this);
}

Reconocedor::~Reconocedor(void) {
	this->recBasicos->removeListener(this);
}

const Gesto * Reconocedor::getUltimoGesto()
{
	Gesto * aux = ultimoGesto;
	ultimoGesto = NULL;
	return aux;
}

const Gesto * Reconocedor::peekUltimoGesto() {
	return ultimoGesto;
}


GestoPatron * Reconocedor::getGestoPatron() {
	return gestoPatron;
}

std::string Reconocedor::getIDJugador_Art() {
	return idJugador_Art;
}

void Reconocedor::addListener(ListenerGesto * listener) {
	listeners.push_back(listener);
}

void Reconocedor::removeListener( ListenerGesto * listener ) {
	std::vector<ListenerGesto*>::iterator it;
	for (it = listeners.begin(); it != listeners.end();) {
		if (*it == listener) {
			listeners.erase(it);
			break;
		}
	}
}

int Reconocedor::getCantListeners() {
	return listeners.size();
}

void Reconocedor::updateMovimiento(Movimiento * m) {
	if (*movimientoEsperado == *m) {
		if (ultimoMovimiento == 0) {
			puntoInicial.set(m->getX(), m->getY(), m->getZ());
			tiempoInicial = m->getTiempo();
		}
		ultimoMovimiento++;
		if (ultimoMovimiento == gestoPatron->length()) {
			std::cout << "Se detecto un Gesto! Notificando... ";
			ultimoGesto = new Gesto(); // falta setearlo (acordarse de pasarle el idJugador_art)
			ultimoGesto->setNombre(gestoPatron->getNombre());
			ultimoGesto->setArticulacion(idJugador_Art);
			ultimoGesto->setTiempo(m->getTiempo());
			Punto3f punto;
			punto.set(m->getX(), m->getY(), m->getZ());
			ultimoGesto->setPosFinal(punto);
			ultimoGesto->setPosInicial(puntoInicial);
			ultimoGesto->setTimeInMillis(difftime(tiempoInicial,ultimoGesto->getTiempo()));
			notifyObservers();
			ultimoMovimiento = 0;
		} else {
			movimientoEsperado = gestoPatron->getMovement(ultimoMovimiento);
		}
	} else {
		ultimoMovimiento = 0;
		movimientoEsperado = gestoPatron->getMovement(ultimoMovimiento);
	}
}

void Reconocedor::notifyObservers() {
	for (unsigned int i = 0; i < listeners.size(); i++)
		listeners.at(i)->updateGesto(ultimoGesto);
}
