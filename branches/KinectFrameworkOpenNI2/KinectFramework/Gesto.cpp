#include "Gesto.h"

const int Gesto::getID()
{
	return id;
}

const Punto3f Gesto::getPosInicial()
{
	return posInicial;
}

const Punto3f Gesto::getPosFinal()
{
	return posFinal;
}

const long int Gesto::getDuracion()
{
	return timeInMillis;
}

const Articulacion * Gesto::getArticulacion()
{
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

const std::time_t Gesto::getTiempo() const
{
	throw std::exception("The method or operation is not implemented.");
}
