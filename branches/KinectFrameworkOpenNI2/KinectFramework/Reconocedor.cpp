#include "Reconocedor.h"


Reconocedor::Reconocedor(GestoPatron * gestoPatron, char* idJugador_Art, XnSkeletonJointTransformation * articulacion, ReconocedorBasico * recBasicos) {
	this->recBasicos = recBasicos;
	this->articulacion = articulacion;
	this->idJugador_Art = idJugador_Art;
	this->gestoPatron = gestoPatron;
	ultimoMovimiento = 0;
	movimientoEsperado = gestoPatron->getMovement(ultimoMovimiento);
	this->recBasicos->addListener(this);
}

Reconocedor::~Reconocedor(void) {

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

char * Reconocedor::getIDJugador_Art(){
	return idJugador_Art;
}

void Reconocedor::addListener(ListenerGesto * listener) {
	listeners.push_back(listener);
}

void Reconocedor::updateMovimiento(Movimiento * m) {
	
	if (movimientoEsperado == m) {
		ultimoMovimiento++;
		if (ultimoMovimiento == gestoPatron->length()) {
			ultimoGesto = new Gesto(); // falta setearlo (acordarse de pasarel el idJugador_art)
			notifyObservers();
		}
	}

}

void Reconocedor::notifyObservers() {
	for (unsigned int i = 0; i < listeners.size(); i++)
		listeners.at(i)->updateGesto(ultimoGesto);
}