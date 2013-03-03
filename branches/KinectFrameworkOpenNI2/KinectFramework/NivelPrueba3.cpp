#include "NivelPrueba3.h"
#include <iostream>

NivelPrueba3::NivelPrueba3(Sensor * kinect, Rendering * rendering) {
	this->kinect = kinect;
	this->rendering = rendering;
}


NivelPrueba3::~NivelPrueba3(void) {

}

void NivelPrueba3::cargar(Escena * nivelAnt) {
	ciclos = 0;
	ciclosGesto = 0;
	cantGestos = 0;
	jugadorActivo = 100000000;
	gestoDetectado = false;
	std::cout << "Cargando NivelPrueba2" << std::endl;
	float i = 0;
	kinect->addListenerJugadorNuevo(this);
	kinect->addListenerJugadorPerdido(this);
	kinect->addListenerJugadorCalibrado(this);
	std::vector<JugadorID> jugadoresActivos = kinect->getJugadoresActivos();
	for (int i = 0; i < jugadoresActivos.size(); i++)
		updateJugadorCalibrado(jugadoresActivos[i]);
	while (i < 30000) i++;
	rendering->setEscena(this);
}

bool NivelPrueba3::isTerminada() {
	return cantGestos > 10;
}

void NivelPrueba3::update() {
	ciclos++;
	if (gestoDetectado)
		ciclosGesto++;
	system("cls");
	std::cout << "Nivel 3 - (" << cantGestos << "/10) gestos detectados." << std::endl;
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
	if (gestoDetectado && ciclosGesto < 30) {
		std::cout << "GESTO DETECTADO" << std::endl;
		std::cout << "   Nombre: " << gesto->getNombre() << std::endl;
		std::cout << "   Desde (" << gesto->getPosInicial().X() << "," 
			<< gesto->getPosInicial().Y() << "," 
			<< gesto->getPosInicial().Z() << ")" << std::endl;
		std::cout << "   Hasta (" << gesto->getPosFinal().X() << ","
			<< gesto->getPosFinal().Y() << ","
			<< gesto->getPosFinal().Z() << ")" << std::endl;
		std::cout << "   Duracion: " << gesto->getDuracion() << "segundos." << std::endl;
	} else {
		ciclosGesto = 0;
		gestoDetectado = false;
	}
}

void NivelPrueba3::updateJugadorNuevo(JugadorID user) {
	std::cout << "Jugador " << user << " encontrado en el nivel 3" << std::endl;
}

Estado NivelPrueba3::getEstadoFinal() {
	return 1;
}

void NivelPrueba3::descargar() {
	bool rec1, rec2;
	rec1 = kinect->stopReconocedor(idRec);
	rec2 = kinect->stopReconocedor(idRec2);
	if (rec1 && rec2)
		std::cout << "Reconocedores detenidos." << std::endl;
	kinect->removeListenerJugadorCalibrado(this);
	kinect->removeListenerJugadorPerdido(this);
	kinect->removeListenerJugadorNuevo(this);
}

void NivelPrueba3::updateJugadorPerdido( JugadorID user ) {
	std::cout << "updateJugadorPerdido: " << user << std::endl;
	kinect->removeListenerGesto(this, idRec);
	for (std::vector<JugadorID>::iterator it = jugadores.begin(); it != jugadores.end();) {
		if (*it == user) {
			jugadores.erase(it);
			break;
		}
	}
}

void NivelPrueba3::updateJugadorCalibrado( JugadorID user ) {
	std::cout << "Jugador " << user << " calibrado." << std::endl;
	jugadorActivo = user;
	jugadores.push_back(user);

	Movimiento arriba;
	arriba.setDireccionX(Movimiento::NO_IMPORTA);
	arriba.setDireccionY(Movimiento::ARRIBA);
	arriba.setDireccionZ(Movimiento::NO_IMPORTA);

	Movimiento abajo;
	abajo.setDireccionX(Movimiento::NO_IMPORTA);
	abajo.setDireccionY(Movimiento::ABAJO);
	abajo.setDireccionZ(Movimiento::NO_IMPORTA);

	Movimiento izquierda;
	izquierda.setDireccionX(Movimiento::IZQUIERDA);
	izquierda.setDireccionY(Movimiento::NO_IMPORTA);
	izquierda.setDireccionZ(Movimiento::NO_IMPORTA);

	Movimiento adelanteArriba;
	adelanteArriba.setDireccionX(Movimiento::SIN_DIRECCION);
	adelanteArriba.setDireccionY(Movimiento::ARRIBA);
	adelanteArriba.setDireccionZ(Movimiento::ADELANTE);

	// Gesto ONDA ONDA CON LA RODILLA
	GestoPatron * gesto = new GestoPatron("ONDA ONDA CON LA RODILLA!!1!uno!");

	gesto->addMovement(arriba);
	gesto->addMovement(abajo);
	gesto->addMovement(arriba);
	gesto->addMovement(abajo);
	idRec = kinect->startReconocedor(user, NiTEEsqueleto::RODILLA_DER, gesto);
	kinect->addListenerGesto(this, idRec);
}

void NivelPrueba3::updateGesto( Gesto * m ) {
	std::cout << " Notificado." << std::endl;
	gestoDetectado = true;
	cantGestos++;
	gesto = m;
}

void NivelPrueba3::getColorFondo( int& r, int& g, int& b ) {
	if (jugadores.size() > 0) {
		const Esqueleto * esq = kinect->getArticulaciones(jugadores[0]);
		if (esq == NULL) return;
		const Articulacion * art = esq->getArticulacion(NiTEEsqueleto::MANO_DER);
		b = ((float)255/(float)1600)*(art->getPosicion()->X()+800);
		g = 0;
		r = 0;
	} else {
		r = 0;
		g = 0;
		b = 0;
	}
}