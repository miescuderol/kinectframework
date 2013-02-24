#include "Sensor.h"

/*
 *	PROTECTED
 */

void Sensor::notifyAllJugadorNuevo(JugadorID jugadorNuevo) {
	m_listenersJugadorNuevo.lock();
	for(unsigned int i = 0; i < listenersJugadorNuevo.size(); i++)
		listenersJugadorNuevo[i]->updateJugadorNuevo(jugadorNuevo);
	m_listenersJugadorNuevo.unlock();
}

void Sensor::notifyAllJugadorPerdido(JugadorID jugadorPerdido) {
	m_listenersJugadorPerdido.lock();
	for (unsigned int i = 0; i < listenersJugadorPerdido.size(); i++)
		listenersJugadorPerdido[i]->updateJugadorPerdido(jugadorPerdido);
	m_listenersJugadorPerdido.unlock();
}

void Sensor::notifyAllJugadorCalibrado( JugadorID jugadorCalibrado ) {
	m_listenersJugadorCalibrado.lock();
	for (unsigned int i = 0; i < listenersJugadorCalibrado.size(); i++)
		listenersJugadorCalibrado[i]->updateJugadorCalibrado(jugadorCalibrado);
	m_listenersJugadorCalibrado.unlock();
}

void Sensor::notifyAllManoNueva(JugadorID manoNueva) {
	m_listenersManoNueva.lock();
	for (unsigned int i = 0; i < listenersManoNueva.size(); i++)
		listenersManoNueva[i]->updateManoNueva(manoNueva);
	m_listenersManoNueva.unlock();
}

void Sensor::notifyAllManoPerdida(JugadorID manoPerdida) {
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
		JugadorID us = atoi(clave.substr(0, clave.find_first_of('_')).data());
		int joint = atoi(clave.substr(clave.find_first_of('_')+1).data());
		Esqueleto * esqueleto = jugadores[us];
		if (esqueleto != NULL)
			it->second->setNewPosition(esqueleto->getArticulacion(joint)->getPosicion()->X(),
			esqueleto->getArticulacion(joint)->getPosicion()->Y(),
			esqueleto->getArticulacion(joint)->getPosicion()->Z());
	}
	m_reconocedoresBasicos.unlock();
}

void Sensor::updateJugadores() {

	if (!isActive(USER_GENERATOR))
		return;

	JugadorID jugadorNuevo;
	if(isNuevoJugador(jugadorNuevo)) {
		std::cout << "nuevo jugador encontrado"<< jugadorNuevo << std::endl;
		notifyAllJugadorNuevo(jugadorNuevo);
	}

	JugadorID jugadorCalibrado;
	if(isJugadorCalibrado(jugadorCalibrado)) {
		std::cout << "nuevo jugador calibrado"<< jugadorCalibrado << std::endl;
		notifyAllJugadorCalibrado(jugadorCalibrado);
	}

	JugadorID jugadorPerdido;
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

	JugadorID idManoAux;
	if (isNuevaMano(idManoAux)) {
		std::cout << "mano nueva encontrada" << idManoAux << std::endl;
		notifyAllManoNueva(idManoAux);
	}
	if(isManoPerdida(idManoAux)) {
		std::cout << "mano nueva perdida" << idManoAux << std::endl;
		notifyAllManoPerdida(idManoAux);
	}

	m_manos.lock();
	Punto3f * mano;
	JugadorID _manoActualizada = manoActualizada(mano);
	if (manos.find(_manoActualizada) != manos.end())	{
		manos[_manoActualizada]->set(mano->X(), mano->Y(), mano->Z());
	}
	m_manos.unlock();
}

void Sensor::run() {
	setup();
	m_isStarted.lock();
	started = true;
	m_isStarted.unlock();
	while (true) {
		update();
		try {
			boost::this_thread::interruption_point();
		} catch (boost::thread_interrupted) {
			stop();
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

std::vector<Sensor::TipoGenerador> Sensor::getActiveGenerators() {
	return generadoresActivos;
}

const Punto3f * Sensor::getMano( JugadorID jugador ) const
{
	if (manos.find(jugador) != manos.end())
		return manos.at(jugador);
	return NULL;
}

const Esqueleto * Sensor::getArticulaciones( JugadorID jugador ) const
{
	if (jugadores.find(jugador) != jugadores.end())
		return jugadores.at(jugador);
	return NULL;
}

int Sensor::startReconocedor( JugadorID jugador, int articulacion, GestoPatron *patron )
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


bool Sensor::stopReconocedor( int idRec ) {
	if(reconocedores.find(idRec) != reconocedores.end())
		return reconocedores.erase(idRec);
	return false;
}


bool Sensor::isNuevoJugador(JugadorID &player) {

	JugadorID jugadorNuevoID = jugadorNuevo();

	if(jugadorNuevoID != -1) {
		player = jugadorNuevoID;
		return true;
	}
	return false;

}

bool Sensor::isJugadorCalibrado(JugadorID &player) {

	Esqueleto * esqueleto;
	JugadorID jugadorCalibradoID = jugadorCalibrado(esqueleto);

	if(jugadorCalibradoID != -1) {
		player = jugadorCalibradoID;
		m_jugadores.lock();
		jugadores[player] = esqueleto; // el [0] no se usa
		m_jugadores.unlock();
		return true;
	}
	return false;
}

bool Sensor::isJugadorPerdido(JugadorID &player) {

	JugadorID jugadorPerdidoID = jugadorPerdido();

	if(jugadorPerdidoID != -1) {
		std::cout << "ID Jugador Perdido: " << jugadorPerdidoID << std::endl;
		player = jugadorPerdidoID;
		m_jugadores.lock();
		if (jugadores.find(player) != jugadores.end())
			jugadores.erase(jugadores.find(player));
		//!\todo Borrar los reconocedores asociados al jugador
		m_jugadores.unlock();
		return true;
	}
	return false;

}

bool Sensor::isNuevaMano(JugadorID &mano) {

	Punto3f * _manoNueva;
	JugadorID manoNuevaID = manoNueva(_manoNueva);

	if (manoNuevaID != -1){
		mano = manoNuevaID;
		m_manos.lock();
		manos[manoNuevaID] = _manoNueva;
		m_manos.unlock();
		return true;
	}
	return false;
}

bool Sensor::isManoPerdida(JugadorID &mano) {
	JugadorID _manoPerdida = manoPerdida();
	if(_manoPerdida != -1){
		mano = _manoPerdida;
		m_manos.lock();
		manos.erase(_manoPerdida);
		m_manos.unlock();
		return true;
	}
	return false;
}

bool Sensor::isStarted() {
	m_isStarted.lock();
	bool start = started;
	m_isStarted.unlock();
	return start;
}

bool Sensor::isActive( TipoGenerador tipo ) {
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

const Gesto * Sensor::getUltimoGestoJugador(JugadorID player) {
	m_reconocedores.lock();
	std::map<int, Reconocedor *>::iterator it = reconocedores.begin();
	std::time_t time = 0; //variable donde almaceno el tiempo del gesto mas actual
	const Gesto * ultimoGesto; 
	const Gesto * gestoAux;
	for(it; it != reconocedores.end(); it++){
		std::string clave = it->second->getIDJugador_Art();
		JugadorID us = atoi(clave.substr(0, clave.find_first_of('_')).data());
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

const Gesto * Sensor::getUltimoGestoReconocedor(int idRec) {
	if(reconocedores.find(idRec) != reconocedores.end())
		return reconocedores[idRec]->getUltimoGesto();
	return NULL;
}

bool Sensor::isTrackingPlayer( JugadorID jugador ) {
	if (jugadores.find(jugador) != jugadores.end())
		return true;
	return false;
}

bool Sensor::isTracking() {
	if (!jugadores.empty())
		return true;
	return false;
}

