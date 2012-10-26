#include "Reconocedor.h"


Reconocedor::Reconocedor(GestoPatron * gestoPatron, char* idJugadorArt, XnSkeletonJointTransformation * articulacion, ReconocedorBasico * recBasicos) {
	this->recBasicos = recBasicos;
	this->articulacion = articulacion;
	//this->idJugador = idJugador;
	//this->joint = joint;
	this->idJugadorArt = idJugadorArt;
	this->gestoPatron = gestoPatron;
	ultimoMovimiento = 0;
	movimientoEsperado = gestoPatron->getMovement(ultimoMovimiento);
	this->recBasicos->addListener(this);
}

Reconocedor::~Reconocedor(void) {

}

Gesto * Reconocedor::getUltimoGesto()
{
	Gesto * aux = ultimoGesto;
	ultimoGesto = NULL;
	return aux;
}
/*
Kinect::Joint Reconocedor::getArticulacion() {
	return joint;
}
*/
GestoPatron * Reconocedor::getGestoPatron() {
	return gestoPatron;
}
/*
XnUserID Reconocedor::getIDJugador() {
	return idJugador;
}
*/
char * Reconocedor::getIDJugadorArt(){
	return idJugadorArt;
}

void Reconocedor::addListener(ListenerGesto * listener) {
	listeners.push_back(listener);
}

void Reconocedor::updateMovimiento(Movimiento * m) {
	
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
		listeners.at(i)->updateGesto(ultimoGesto);
}