#include "Reconocedor.h"


Reconocedor::Reconocedor(GestoPatron * gestoPatron, XnSkeletonJointTransformation * articulacion, std::vector<ReconocedorBasico*> * recBasicos) {
	this->recBasicos = recBasicos;
	gestoTemplate = gestoPatron;
	ultimoMovimiento = 0;
	movimientoEsperado = gestoTemplate->getMovement(ultimoMovimiento);
}

Reconocedor::~Reconocedor(void) {

}

Gesto * Reconocedor::lastGesture() {
	Gesto * aux = ultimoGesto;
	ultimoGesto = NULL;
	return aux;
}

void Reconocedor::addListener(ListenerGesto * listener) {
	listeners.push_back(listener);
}

void Reconocedor::update(Movimiento * m) {
	
	if (movimientoEsperado == m) {
		ultimoMovimiento++;
		if (ultimoMovimiento == gestoTemplate->length()) {
			ultimoGesto = new Gesto(); // falta setearlo
			notifyObservers();
		}
	}

}

void Reconocedor::notifyObservers() {
	for (int i = 0; i < listeners.size(); i++)
		listeners.at(i)->update(ultimoGesto);
}