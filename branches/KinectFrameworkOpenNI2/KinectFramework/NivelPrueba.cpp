#include "NivelPrueba.h"
#include "MiAplicacion.h"
#include <iostream>


NivelPrueba::NivelPrueba(Sensor * kinect) {
	this->kinect = kinect;
	ciclos = 0;
	ciclosGesto = 0;
	jugadorActivo = 100000000;
	gestoDetectado = false;
}


NivelPrueba::~NivelPrueba(void) {

}

void NivelPrueba::cargar(Escena * nivelAnt) {
	std::cout << "Cargando NivelPrueba" << std::endl;
	float i = 0;
	kinect->addListenerJugadorNuevo(this);
	kinect->addListenerJugadorPerdido(this);
	kinect->addListenerJugadorCalibrado(this);
	while (i < 30000) i++;
}

bool NivelPrueba::isTerminada() {
	return false;
}

void NivelPrueba::update() {
	ciclos++;
	if (gestoDetectado)
		ciclosGesto++;
	//system("cls");
	std::cout << "Nivel 1" << std::endl;
	std::cout << "Jugadores: ";
	for (int i = 0; i < jugadores.size(); i++)
		std::cout << jugadores[i] << " ";
	std::cout << std::endl;
	for (int i = 0; i < jugadores.size(); i++) {
		const Esqueleto * esq = kinect->getArticulaciones(jugadores[i]);
		if (esq == NULL) return;
		const Articulacion * art = esq->getArticulacion(NiTEEsqueleto::MANO_DER);
		std::cout << "Jugador " << jugadores[i] << ": " << art->getPosicion()->X() << ", "
														<< art->getPosicion()->Y() << ", "
														<< art->getPosicion()->Z() << std::endl;
	}
	if (gestoDetectado && ciclosGesto < 30)
		std::cout << "GESTO DETECTADO" << std::endl;
	else {
		ciclosGesto = 0;
		gestoDetectado = false;
	}
	if (ciclos > 50) {
		int c;
		std::cin >> c;
	}
}

void NivelPrueba::updateJugadorNuevo(JugadorID user) {
	std::cout << "Jugador " << user << " encontrado en el nivel 1" << std::endl;
}

Estado NivelPrueba::getEstadoFinal() {
	return 1;
}

void NivelPrueba::descargar()
{

}

void NivelPrueba::updateJugadorPerdido( JugadorID user )
{
	std::cout << "updateJugadorPerdido: " << user << std::endl;
	kinect->removeListenerGesto(this, idRec);
	for (std::vector<JugadorID>::iterator it = jugadores.begin(); it != jugadores.end();) {
		if (*it == user) {
			jugadores.erase(it);
			break;
		}
	}
}

void NivelPrueba::updateJugadorCalibrado( JugadorID user )
{
	std::cout << "Jugador " << user << " calibrado." << std::endl;
	jugadorActivo = user;
	jugadores.push_back(user);
	GestoPatron * gesto = new GestoPatron();
	Movimiento arriba;
	arriba.setDireccionHorizontal(0);
	gesto->addMovement(arriba);
	idRec = kinect->startReconocedor(user, NiTEEsqueleto::MANO_DER, gesto);
	kinect->addListenerGesto(this, idRec);
}

void NivelPrueba::updateGesto( Gesto * m )
{
	std::cout << " Notificado." << std::endl;
	gestoDetectado = true;
}
