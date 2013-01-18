#include "Sensor.h"

/*
 *	PROTECTED
 */

void Sensor::notifyAllJugadorNuevo(XnUserID jugadorNuevo) {
	m_listenersJugadorNuevo.lock();
	for(unsigned int i = 0; i < listenersJugadorNuevo.size(); i++)
		listenersJugadorNuevo[i]->updateJugadorNuevo(jugadorNuevo);
	m_listenersJugadorNuevo.unlock();
}

void Sensor::notifyAllJugadorPerdido(XnUserID jugadorPerdido) {
	m_listenersJugadorPerdido.lock();
	for (unsigned int i = 0; i < listenersJugadorPerdido.size(); i++)
		listenersJugadorPerdido[i]->updateJugadorPerdido(jugadorPerdido);
	m_listenersJugadorPerdido.unlock();
}

void Sensor::notifyAllJugadorCalibrado( XnUserID jugadorCalibrado ) {
	m_listenersJugadorCalibrado.lock();
	for (unsigned int i = 0; i < listenersJugadorCalibrado.size(); i++)
		listenersJugadorCalibrado[i]->updateJugadorCalibrado(jugadorCalibrado);
	m_listenersJugadorCalibrado.unlock();
}

void Sensor::notifyAllManoNueva(XnUserID manoNueva) {
	m_listenersManoNueva.lock();
	for (unsigned int i = 0; i < listenersManoNueva.size(); i++)
		listenersManoNueva[i]->updateManoNueva(manoNueva);
	m_listenersManoNueva.unlock();
}

void Sensor::notifyAllManoPerdida(XnUserID manoPerdida) {
	m_listenersManoPerdida.lock();
	for (unsigned int i = 0; i < listenersManoPerdida.size(); i++)
		listenersManoPerdida[i]->updateManoPerdida(manoPerdida);
	m_listenersManoPerdida.unlock();
}

ReconocedorBasico * Sensor::buscarReconocedorBasico(char * idRecBasico) {
	m_reconocedoresBasicos.lock();
	if(reconocedoresBasicos.find(idRecBasico) == reconocedoresBasicos.end()){
		reconocedoresBasicos[idRecBasico] = new ReconocedorBasico(8, 70); //cambiar los parámetros por constantes
	}
	ReconocedorBasico * reconocedorBasicoAux;
	reconocedorBasicoAux = reconocedoresBasicos[idRecBasico];
	m_reconocedoresBasicos.unlock();
	return reconocedorBasicoAux;
}

void Sensor::updateReconocedoresBasicos() {
	m_reconocedoresBasicos.lock();
	std::map<char*, ReconocedorBasico*>::iterator it = reconocedoresBasicos.begin();
	for(it; it != reconocedoresBasicos.end(); it++){
		std::string clave = it->first;
		XnUserID us = atoi(clave.substr(0, clave.find_first_of('_')).data());
		int joint = atoi(clave.substr(clave.find_first_of('_')+1).data());
		Esqueleto * esqueleto = jugadores[us];
		it->second->setNewPosition(esqueleto->getArticulacion(joint)->getPosicion()->X(),
								   esqueleto->getArticulacion(joint)->getPosicion()->Y(),
								   esqueleto->getArticulacion(joint)->getPosicion()->Z());
	}
	m_reconocedoresBasicos.unlock();
}

void Sensor::updateJugadores() {

	if (!isActive(USER_GENERATOR))
		return;

	XnUserID jugadorNuevo;
	if(isNuevoJugador(jugadorNuevo)) {
		std::cout << "nuevo jugador encontrado"<< jugadorNuevo << std::endl;
		notifyAllJugadorNuevo(jugadorNuevo);
	}

	XnUserID jugadorCalibrado;
	if(isJugadorCalibrado(jugadorCalibrado)) {
		std::cout << "nuevo jugador calibrado"<< jugadorCalibrado << std::endl;
		notifyAllJugadorCalibrado(jugadorCalibrado);
	}

	XnUserID jugadorPerdido;
	if(isJugadorPerdido(jugadorPerdido)) {
		std::cout << "jugador perdido"<< jugadorPerdido << std::endl;
		notifyAllJugadorPerdido(jugadorPerdido);
	}

	m_jugadores.lock();
	updateArticulacionesJugadores();
	m_jugadores.unlock();
}

void Sensor::updateManos() {

	if (!isActive(HAND_GENERATOR))
		return;

	XnUserID idManoAux;
	if (isNuevaMano(idManoAux)) {
		std::cout << "mano nueva encontrada" << idManoAux << std::endl;
		notifyAllManoNueva(idManoAux);
	}
	if(isManoPerdida(idManoAux)) {
		std::cout << "mano nueva perdida" << idManoAux << std::endl;
		notifyAllManoPerdida(idManoAux);
	}

	m_manos.lock();
	XnPoint3D * mano;
	XnUserID manoActualizada = manoActualizada(mano);
	if (manos.find(manoActualizada) != manos.end())	{
		manos[manoActualizada]->X = mano->X;
		manos[manoActualizada]->Y = mano->Y;
		manos[manoActualizada]->Z = mano->Z;
	}
	m_manos.unlock();
}

void Sensor::run() {
	setup();

	while (true) {
		update();
		try {
			boost::this_thread::interruption_point();
		} catch (boost::thread_interrupted) {
			break;
		}
	}
}

/*
 *	PUBLIC
 */

Sensor::Sensor() {
	started = false;
}

void Sensor::start() {
	threadKinect = boost::thread(&Sensor::run, this);
}

void Sensor::shutdown() {
	threadKinect.interrupt();
}

std::vector<GeneratorType> Sensor::getActiveGenerators() {
	return generadoresActivos;
}

int Sensor::startReconocedor( XnUserID jugador, int articulacion, GestoPatron *patron )
{
	if (!isTrackingPlayer(jugador))
		return -1;

	char* idRecBasico = new char(jugador + '_' + articulacion);
	ReconocedorBasico *recBasico = buscarReconocedorBasico(idRecBasico);

	m_reconocedores.lock();
	unsigned int i = 0;
	int idRec = -1;
	for(i; (i < reconocedores.size()) && (idRec == -1); i++) {
		Reconocedor *r = reconocedores.at(i);
		if(r->getIDJugador_Art() == idRecBasico && r->getGestoPatron() == patron)
			idRec = i;
	}
	if(i == reconocedores.size()) {
		idRec = i;
		Reconocedor *nuevo = new Reconocedor(patron, idRecBasico, recBasico);
		reconocedores[i] = nuevo;
	}
	m_reconocedores.unlock();
	return idRec;
}

bool Sensor::isNuevoJugador(XnUserID &player) {

	XnUserID jugadorNuevoID = jugadorNuevo();

	if(nuevoJugadorID != -1) {
		player = nuevoJugadorID;
		return true;
	}
	return false;

}

bool Sensor::isJugadorCalibrado(XnUserID &player) {

	Esqueleto * esqueleto;
	XnUserID jugadorCalibradoID = jugadorCalibrado(esqueleto);

	if(jugadorCalibradoID != -1) {
		player = jugadorCalibradoID;
		m_jugadores.lock();
		jugadores[player] = esqueleto; // el [0] no se usa
		m_jugadores.unlock();
		return true;
	}
	return false;
}

bool Sensor::isJugadorPerdido(XnUserID &player) {

	XnUserID jugadorPerdidoID = jugadorPerdido();

	if(jugadorPerdidoID != -1) {
		player = jugadorPerdidoID;
		m_jugadores.lock();
		jugadores.erase(jugadores.find(player));
		//!\todo Borrar los reconocedores asociados al jugador
		m_jugadores.unlock();
		return true;
	}
	return false;

}

bool Sensor::isNuevaMano(XnUserID &mano) {

	XnPoint3D * manoNueva;
	XnUserID manoNuevaID = manoNueva(manoNueva);

	if (manoNuevaID != -1){
		mano = manoNuevaID;
		m_manos.lock();
		manos[manoNuevaID] = manoNueva;
		m_manos.unlock();
		return true;
	}
	return false;
}

bool Sensor::isManoPerdida(XnUserID &mano) {
	XnUserID manoPerdida = manoPerdida();
	if(manoPerdida != -1){
		mano = manoPerdida;
		m_manos.lock();
		manos.erase(manoPerdida);
		m_manos.unlock();
		return true;
	}
	return false;
}

bool Sensor::isStarted() {
	return started;
}

bool Sensor::isActive( GeneratorType tipo )
{
	for (unsigned int i = 0; i < generadoresActivos.size(); i++)
		if (generadoresActivos.at(i) == tipo)
			return true;
	return false;
}

//Agregar Listeners//

void Sensor::addListenerGesto(ListenerGesto *lg, int idRec) {
	m_reconocedores.lock();
	if (reconocedores.find(idRec) != reconocedores.end())
		reconocedores[idRec]->addListener(lg);
	m_reconocedores.unlock();
}

void Sensor::addListenerJugadorNuevo(ListenerJugadorNuevo *lnj) {
	m_listenersJugadorNuevo.lock();
	listenersJugadorNuevo.push_back(lnj);
	m_listenersJugadorNuevo.unlock();
}

void Sensor::addListenerJugadorPerdido(ListenerJugadorPerdido *ljp) {
	m_listenersJugadorPerdido.lock();
	listenersJugadorPerdido.push_back(ljp);
	m_listenersJugadorPerdido.unlock();
}

void Sensor::addListenerJugadorCalibrado(ListenerJugadorCalibrado *ljc) {
	m_listenersJugadorCalibrado.lock();
	listenersJugadorCalibrado.push_back(ljc);
	m_listenersJugadorCalibrado.unlock();
}

void Sensor::addListenerManoNueva(ListenerManoNueva *lmn) {
	m_listenersManoNueva.lock();
	listenersManoNueva.push_back(lmn);
	m_listenersManoNueva.unlock();
}

void Sensor::addListenerManoPerdida(ListenerManoPerdida *lmp) {
	m_listenersManoPerdida.lock();
	listenersManoPerdida.push_back(lmp);
	m_listenersManoPerdida.unlock();
}

const Gesto * Sensor::getUltimoGesto(XnUserID player) {
	m_reconocedores.lock();
	std::map<int, Reconocedor *>::iterator it = reconocedores.begin();
	std::time_t time = 0; //variable donde almaceno el tiempo del gesto mas actual
	const Gesto * ultimoGesto; 
	const Gesto * gestoAux;
	for(it; it != reconocedores.end(); it++){
		std::string clave = it->second->getIDJugador_Art();
		XnUserID us = atoi(clave.substr(0, clave.find_first_of('_')).data());
		if(us == player){
			gestoAux = it->second->peekUltimoGesto();
			if(gestoAux->getTiempo() > time){
				ultimoGesto = gestoAux;
				time = gestoAux->getTiempo();
			}
		}
	}
	m_reconocedores.unlock();
	return ultimoGesto;
}

const Gesto * Sensor::getUltimoGesto(int idRec) {
	return reconocedores[idRec]->getUltimoGesto();
}

