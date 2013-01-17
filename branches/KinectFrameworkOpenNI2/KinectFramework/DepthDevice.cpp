#include "DepthDevice.h"

/*
 *	PROTECTED
 */

void DepthDevice::notifyAllJugadorNuevo(XnUserID jugadorNuevo) {
	m_listenersJugadorNuevo.lock();
	for(unsigned int i = 0; i < listenersJugadorNuevo.size(); i++)
		listenersJugadorNuevo[i]->updateJugadorNuevo(jugadorNuevo);
	m_listenersJugadorNuevo.unlock();
}

void DepthDevice::notifyAllJugadorPerdido(XnUserID jugadorPerdido) {
	m_listenersJugadorPerdido.lock();
	for (unsigned int i = 0; i < listenersJugadorPerdido.size(); i++)
		listenersJugadorPerdido[i]->updateJugadorPerdido(jugadorPerdido);
	m_listenersJugadorPerdido.unlock();
}

void DepthDevice::notifyAllJugadorCalibrado( XnUserID jugadorCalibrado ) {
	m_listenersJugadorCalibrado.lock();
	for (unsigned int i = 0; i < listenersJugadorCalibrado.size(); i++)
		listenersJugadorCalibrado[i]->updateJugadorCalibrado(jugadorCalibrado);
	m_listenersJugadorCalibrado.unlock();
}

void DepthDevice::notifyAllManoNueva(XnUserID manoNueva) {
	m_listenersManoNueva.lock();
	for (unsigned int i = 0; i < listenersManoNueva.size(); i++)
		listenersManoNueva[i]->updateManoNueva(manoNueva);
	m_listenersManoNueva.unlock();
}

void DepthDevice::notifyAllManoPerdida(XnUserID manoPerdida) {
	m_listenersManoPerdida.lock();
	for (unsigned int i = 0; i < listenersManoPerdida.size(); i++)
		listenersManoPerdida[i]->updateManoPerdida(manoPerdida);
	m_listenersManoPerdida.unlock();
}

ReconocedorBasico * DepthDevice::buscarReconocedorBasico(char * idRecBasico) {
	m_reconocedoresBasicos.lock();
	if(reconocedoresBasicos.find(idRecBasico) == reconocedoresBasicos.end()){
		reconocedoresBasicos[idRecBasico] = new ReconocedorBasico(8, 70); //cambiar los parámetros por constantes
	}
	ReconocedorBasico * reconocedorBasicoAux;
	reconocedorBasicoAux = reconocedoresBasicos[idRecBasico];
	m_reconocedoresBasicos.unlock();
	return reconocedorBasicoAux;
}

void DepthDevice::updateReconocedoresBasicos() {
	m_reconocedoresBasicos.lock();
	std::map<char*, ReconocedorBasico*>::iterator it = reconocedoresBasicos.begin();
	for(it; it != reconocedoresBasicos.end(); it++){
		std::string clave = it->first;
		XnUserID us = atoi(clave.substr(0, clave.find_first_of('_')).data());
		Joint joint = (Joint)atoi(clave.substr(clave.find_first_of('_')+1).data());
		XnSkeletonJointTransformation * joints = jugadores[us];
		it->second->setNewPosition(joints[joint].position.position.X, joints[joint].position.position.Y, joints[joint].position.position.Z);
	}
	m_reconocedoresBasicos.unlock();
}

void DepthDevice::updateJugadores() {

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
	std::map<XnUserID, XnSkeletonJointTransformation* >::iterator it = jugadores.begin();
	for(it; it != jugadores.end(); it++) {
		if (isTrackingPlayer(it->first))
			//for (int i = 1; i <= 24; i++) { // 24 = cantidad de Kinect::Joint 
			//	XnSkeletonJointTransformation j;
			//	userG.GetSkeletonCap().GetSkeletonJoint(it->first, (XnSkeletonJoint)i, j);
			//	//		if(j.position.fConfidence > minConfidence)
			//	(it->second[i]) = j;
			//}
			updateArticulacionesJugador(it->first);
	}
	m_jugadores.unlock();
}

void DepthDevice::updateManos() {
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

void DepthDevice::run() {
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

DepthDevice::DepthDevice() {
	started = false;
}

void DepthDevice::start() {
	threadKinect = boost::thread(&DepthDevice::run, this);
}

void DepthDevice::shutdown() {
	threadKinect.interrupt();
}

int DepthDevice::startReconocedor(XnUserID jugador, Joint articulacion, GestoPatron *patron) {
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

bool DepthDevice::isNuevoJugador(XnUserID &player) {
	if(nuevoJugadorID != -1) {
		player = nuevoJugadorID;
		nuevoJugadorID = -1;
		return true;
	}
	return false;
}

bool DepthDevice::isJugadorCalibrado(XnUserID &player) {
	if(jugadorCalibradoID != -1) {
		player = jugadorCalibradoID;
		jugadorCalibradoID = -1;
		m_jugadores.lock();
		jugadores[player] = new XnSkeletonJointTransformation[25]; // el [0] no se usa
		m_jugadores.unlock();
		return true;
	}
	return false;
}

bool DepthDevice::isJugadorPerdido(XnUserID &player) {
	if(jugadorPerdidoID != -1) {
		player = jugadorPerdidoID;
		jugadorPerdidoID = -1;
		m_jugadores.lock();
		jugadores.erase(player);
		m_jugadores.unlock();
		return true;
	}
	return false;
}

bool DepthDevice::isNuevaMano(XnUserID &mano) {

	XnPoint3D * manoNueva;
	XnUserID manoNuevaID = manoNuevaID(manoNueva);

	if (manoNuevaID != -1){
		mano = manoNuevaID;
		m_manos.lock();
		manos[manoNuevaID] = manoNueva;
		m_manos.unlock();
		return true;
	}
	return false;
}

bool DepthDevice::isManoPerdida(XnUserID &mano) {
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

bool DepthDevice::isStarted() {
	return started;
}

//Agregar Listeners//

void DepthDevice::addListenerGesto(ListenerGesto *lg, int idRec) {
	m_reconocedores.lock();
	if (reconocedores.find(idRec) != reconocedores.end())
		reconocedores[idRec]->addListener(lg);
	m_reconocedores.unlock();
}

void DepthDevice::addListenerJugadorNuevo(ListenerJugadorNuevo *lnj) {
	m_listenersJugadorNuevo.lock();
	listenersJugadorNuevo.push_back(lnj);
	m_listenersJugadorNuevo.unlock();
}

void DepthDevice::addListenerJugadorPerdido(ListenerJugadorPerdido *ljp) {
	m_listenersJugadorPerdido.lock();
	listenersJugadorPerdido.push_back(ljp);
	m_listenersJugadorPerdido.unlock();
}

void DepthDevice::addListenerJugadorCalibrado(ListenerJugadorCalibrado *ljc) {
	m_listenersJugadorCalibrado.lock();
	listenersJugadorCalibrado.push_back(ljc);
	m_listenersJugadorCalibrado.unlock();
}

void DepthDevice::addListenerManoNueva(ListenerManoNueva *lmn) {
	m_listenersManoNueva.lock();
	listenersManoNueva.push_back(lmn);
	m_listenersManoNueva.unlock();
}

void DepthDevice::addListenerManoPerdida(ListenerManoPerdida *lmp) {
	m_listenersManoPerdida.lock();
	listenersManoPerdida.push_back(lmp);
	m_listenersManoPerdida.unlock();
}

const Gesto * DepthDevice::getUltimoGesto(XnUserID player) {
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

const Gesto * DepthDevice::getUltimoGesto(int idRec) {
	return reconocedores[idRec]->getUltimoGesto();
}

