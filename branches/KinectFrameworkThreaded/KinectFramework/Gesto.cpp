#include "Gesto.h"

int Gesto::getID() {
	return id;
}

XnPoint3D Gesto::getPosInicial() {
	return posInicial;
}

XnPoint3D Gesto::getPosFinal() {
	return posFinal;
}

long int Gesto::getDuracion()
{
	return timeInMillis;
}

const XnSkeletonJointTransformation * Gesto::getArticulacion() {
	return articulacion;
}

Gesto * Gesto::setID(int id) {
	this->id = id;
	return this;
}

Gesto * Gesto::setPosInicial(XnPoint3D pos) {
	posInicial = pos;
	return this;
}

Gesto * Gesto::setPosFinal(XnPoint3D pos) {
	posFinal = pos;
	return this;
}

Gesto * Gesto::setTimeInMillis(long int time) {
	timeInMillis = time;
	return this;
}

Gesto * Gesto::setArticulacion(const XnSkeletonJointTransformation * articulacion) {
	this->articulacion = articulacion;
	return this;
}

bool Gesto::getTiempo() const
{
	throw std::exception("The method or operation is not implemented.");
}
