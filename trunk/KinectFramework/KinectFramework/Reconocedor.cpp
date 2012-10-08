#include "Reconocedor.h"


Reconocedor::Reconocedor(GestoPatron * gestoPatron, XnUserID * idJugador, XnSkeletonJointTransformation * articulacion, ReconocedorBasico * recBasicos) {
	this->recBasicos = recBasicos;
	this->articulacion = articulacion;
	this->idJugador = idJugador;
	this->gestoPatron = gestoPatron;
	ultimoMovimiento = 0;
	movimientoEsperado = gestoPatron->getMovement(ultimoMovimiento);
}

Reconocedor::~Reconocedor(void) {

}

Gesto * Reconocedor::getUltimoGesto()
{
	Gesto * aux = ultimoGesto;
	ultimoGesto = NULL;
	return aux;
}

Kinect::Joint * Reconocedor::getArticulacion() {
	return articulacion;
}

GestoPatron * Reconocedor::getGestoPatron() {
	return gestoPatron;
}

XnUserID * Reconocedor::getIDJugador() {
	return idJugador;
}

void Reconocedor::addListener(ListenerGesto * listener) {
	listeners.push_back(listener);
}

void Reconocedor::update(Movimiento * m) {
	
	if (movimientoEsperado == m) {
		ultimoMovimiento++;
		if (ultimoMovimiento == gestoPatron->length()) {
			ultimoGesto = new Gesto(); // falta setearlo
			notifyObservers();
		}
	}

}

void Reconocedor::notifyObservers() {
	for (int i = 0; i < listeners.size(); i++)
		listeners.at(i)->update(ultimoGesto);
}