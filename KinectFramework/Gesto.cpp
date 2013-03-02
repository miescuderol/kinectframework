#include "Gesto.h"

Gesto::Gesto() {

}

Gesto::~Gesto() {

}

const std::string Gesto::getNombre() {
	return nombre;
}

const Punto3f Gesto::getPosInicial() {
	return posInicial;
}

const Punto3f Gesto::getPosFinal() {
	return posFinal;
}

const long int Gesto::getDuracion() {
	return timeInMillis;
}

const std::string Gesto::getArticulacion() {
	return articulacion;
}

const std::time_t Gesto::getTiempo() const {
	return tiempo;
}

Gesto * Gesto::setNombre(std::string nombre) {
	this->nombre = nombre;
	return this;
}

Gesto * Gesto::setPosInicial(Punto3f pos) {
	posInicial = pos;
	return this;
}

Gesto * Gesto::setPosFinal(Punto3f pos) {
	posFinal = pos;
	return this;
}

Gesto * Gesto::setTimeInMillis(long int time) {
	timeInMillis = time;
	return this;
}

Gesto * Gesto::setArticulacion(const std::string idJugador_articulacion) {
	this->articulacion.clear();
	this->articulacion.append(idJugador_articulacion);

	return this;
}

Gesto * Gesto::setTiempo( std::time_t t ) {
	this->tiempo = t;
	return this;
}