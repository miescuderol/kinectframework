#define DEBUG 0

#include "Kinect.h"

	xn::GestureGenerator gestureG; 
	xn::HandsGenerator handG; 
	xn::DepthGenerator depthG;
	xn::ImageGenerator imageG;
	xn::UserGenerator userG;
	
	XnUserID nuevoJugadorID = -1;
	XnUserID jugadorPerdidoID = -1;
	XnUserID jugadorCalibradoID = -1;
	XnBoundingBox3D* boundingBox; 
	const XnChar * recognizedGesture;
	XnUserID nuevaManoID = -1;
	XnUserID manoPerdidaID = -1;
	XnUserID manoActualizada;
	XnPoint3D * mano, * nuevaMano;
	XnSkeletonJointPosition * Head = new XnSkeletonJointPosition; 
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//
	//		CALLBACKS DE GESTOS
	//

	// HANDLERS
	XnCallbackHandle hGesture, hHand, hUser, hPose, hCalibration;
	
	void XN_CALLBACK_TYPE Gesture_Recognized(xn::GestureGenerator& generator, 
					   const XnChar* strGesture, 
					   const XnPoint3D* pIDPosition, 
					   const XnPoint3D* pEndPosition, void* pCookie){ 
	  printf("Gesture recognized: %s\n", strGesture); 
	  recognizedGesture = strGesture;
	  handG.StartTracking(*pEndPosition); 
	}

	void XN_CALLBACK_TYPE Gesture_Process(xn::GestureGenerator& generator, 
					const XnChar* strGesture, 
					const XnPoint3D* pPosition, 
					XnFloat fProgress, 
					void* pCookie){
	} 

	//
	//		CALLBACKS DE MANOS
	//
 
	void XN_CALLBACK_TYPE Hand_Create(xn::HandsGenerator& generator, 
				XnUserID nId, const XnPoint3D* pPosition, 
				XnFloat fTime, void* pCookie){ 
	  printf("New Hand: %d @ (%f,%f,%f)\n", nId, 
			 pPosition->X, pPosition->Y, pPosition->Z); 
		nuevaManoID = nId;
		nuevaMano = new XnPoint3D;
		nuevaMano->X = pPosition->X;
		nuevaMano->Y = pPosition->Y;
		nuevaMano->Z = pPosition->Z;
	} 
	void XN_CALLBACK_TYPE Hand_Update(xn::HandsGenerator& generator, 
				XnUserID nId, const XnPoint3D* pPosition, 
				XnFloat fTime, void* pCookie){
		if (!mano) 
			mano = new XnPoint3D;
		manoActualizada = nId;
		mano->X = pPosition->X;
		mano->Y = pPosition->Y;
		mano->Z = pPosition->Z;
	}

	void XN_CALLBACK_TYPE Hand_Destroy(xn::HandsGenerator& generator, 
				 XnUserID nId, XnFloat fTime, 
				 void* pCookie){ 

	  printf("Lost Hand: %d\n", nId); 
	  manoPerdidaID = nId;
	}

	//
	//		CALLBACKS DE USER GENERATOR
	//

	void XN_CALLBACK_TYPE User_NewUser(xn::UserGenerator& generator, 
				 XnUserID nId, void* pCookie){ 
		printf("New User: %d\n", nId); 
		nuevoJugadorID = nId;
		userG.GetPoseDetectionCap().StartPoseDetection("Psi", nId); 
	} 

	void XN_CALLBACK_TYPE User_LostUser(xn::UserGenerator& generator, 
					XnUserID nId, void* pCookie){
		jugadorPerdidoID = nId;
	} 

	void XN_CALLBACK_TYPE Pose_Detected(xn::PoseDetectionCapability& pose, 
					const XnChar* strPose, XnUserID nId, void* pCookie){ 
		  printf("Pose %s for user %d\n", strPose, nId); 
		  userG.GetPoseDetectionCap().StopPoseDetection(nId); 
		  userG.GetSkeletonCap().RequestCalibration(nId, TRUE); 
	} 

	void XN_CALLBACK_TYPE Calibration_Start(xn::SkeletonCapability& capability, 
					XnUserID nId, void* pCookie){ 
		printf("Starting calibration for user %d\n", nId); 
	} 

	void XN_CALLBACK_TYPE Calibration_End(xn::SkeletonCapability& capability, XnUserID nId, XnBool bSuccess, void* pCookie){
		if (bSuccess) 
		{ 
			printf("User calibrated\n"); 
			jugadorCalibradoID = nId;
			std::cout << "-----------------------------------------> " << jugadorCalibradoID << std::endl;
			userG.GetSkeletonCap().StartTracking(nId); 
		} 
		else 
		{ 
			printf("Failed to calibrate user %d\n", nId); 
		    userG.GetPoseDetectionCap().StartPoseDetection("Psi", nId); 
		}
	}

	//
	//		FIN CALLBACKS
	//

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

Kinect::Kinect(void) {

}


Kinect::~Kinect(void) {

}

void Kinect::setup() {

	bool sin_error = checkStatusOK(contexto.Init(), "Init context") &&

	checkStatusOK(depthG.Create(contexto), "Crear depthG") &&

	checkStatusOK(imageG.Create(contexto), "Crear imageG") &&
	
	checkStatusOK(gestureG.Create(contexto), "Crear gestureG") &&
	
	checkStatusOK(handG.Create(contexto), "Crear handG") &&

	checkStatusOK(userG.Create(contexto), "Crear userG");


	escena = new SceneMetaData();

	if (!sin_error) {
		int i;
	std::cin >> i;
		
	}

	enableGenerator(Kinect::DEPTH_GENERATOR);
	enableGenerator(Kinect::USER_GENERATOR);
}

bool Kinect::enableGenerator(GeneratorType generator) {
	switch(generator) {
		case IMAGE_GENERATOR: 
			return checkStatusOK(imageG.StartGenerating(), "StartGenerating imageG");
			break;
		case DEPTH_GENERATOR: 
			return checkStatusOK(depthG.StartGenerating(), "StartGenerating depthG");
			break; 
		case USER_GENERATOR: 
			return checkStatusOK(userG.RegisterUserCallbacks(User_NewUser, User_LostUser, NULL, hUser), "Callbacks User") &&
					checkStatusOK(userG.GetPoseDetectionCap().RegisterToPoseCallbacks(Pose_Detected, NULL, NULL, hPose), "Callbacks Pose") &&
					checkStatusOK(userG.GetSkeletonCap().RegisterCalibrationCallbacks(Calibration_Start, Calibration_End, NULL, hCalibration), "Callbacks Calibration") &&
					checkStatusOK(userG.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL), "Skeleton Profile") &&
					checkStatusOK(userG.GetSkeletonCap().SetSmoothing(0.4), "Smoothing") &&
					checkStatusOK(userG.StartGenerating(), "StartGenerating userG");
			break; 
		case HAND_GENERATOR: 
			return checkStatusOK(handG.RegisterHandCallbacks(Hand_Create, Hand_Update, Hand_Destroy, NULL, hHand), "Callbacks Hand") &&
				checkStatusOK(handG.StartGenerating(), "StartGenerating handG");
			break;
		case GESTURE_GENERATOR: 
			return checkStatusOK(gestureG.RegisterGestureCallbacks(Gesture_Recognized, Gesture_Process, NULL, hGesture), "Callbacks Gesture") && 
					checkStatusOK(gestureG.AddGesture("Click", boundingBox), "Agregar gesto click") &&
					checkStatusOK(gestureG.AddGesture("Wave", boundingBox), "Agregar gesto wave") &&
					checkStatusOK(gestureG.StartGenerating(), "StartGenerating gestureG");
			break;
		case ALL_GENERATORS: 
			enableGenerator(IMAGE_GENERATOR);
			enableGenerator(DEPTH_GENERATOR);
			enableGenerator(USER_GENERATOR);
			enableGenerator(HAND_GENERATOR);
			enableGenerator(GESTURE_GENERATOR);
			break;
	}
}

bool Kinect::disableGenerator(GeneratorType generator) {
	return false;
}

XnChar * Kinect::getActiveGenerators() {
	return NULL;
}

void Kinect::update() {

	m_generadores.lock();
	bool ok = checkStatusOK(contexto.WaitAnyUpdateAll(), "Wait and Update all");
	m_generadores.unlock();

	if (!ok)
		return;

	// va sin semaforo porque no hay race conditions
	mapaProfundidad = depthG.GetDepthMap();

	mapaImagen = imageG.GetImageMap();

	updateJugadores();

	updateReconocedoresBasicos();

	updateManos();
	
}

bool Kinect::checkStatusOK(const XnStatus status, char* entorno) {
	if (status != XN_STATUS_OK) {
		std::cout << xnGetStatusString(status) << " " << entorno << std::endl;
		return false;
	}
	return true;
}

const int Kinect::getXRes() {
	DepthMetaData m;
	m_generadores.lock();
	depthG.GetMetaData(m);
	m_generadores.unlock();
	return m.XRes();
}

const int Kinect::getYRes() {
	DepthMetaData m;
	m_generadores.lock();
	depthG.GetMetaData(m);
	m_generadores.unlock();
	return m.YRes();
}

const XnPoint3D * Kinect::getMano(XnUserID jugador) {
	m_manos.lock();
	const XnPoint3D * aux = manos.find(jugador) != manos.end() ? 
		manos[jugador] : NULL;
	m_manos.unlock();
	return aux;
}

const XnSkeletonJointTransformation * Kinect::getArticulaciones(XnUserID jugador) {
	m_jugadores.lock();
	const XnSkeletonJointTransformation * aux = jugadores.find(jugador) != jugadores.end() ? 
		jugadores[jugador] : NULL;
	m_jugadores.unlock();
	return aux;
}

bool Kinect::isTrackingPlayer(XnUserID player) {
	m_generadores.lock();
	bool b = userG.IsGenerating() && userG.IsCapabilitySupported(XN_CAPABILITY_SKELETON) && userG.GetSkeletonCap().IsTracking(player);
	m_generadores.unlock();
	return b;
}

bool Kinect::isTracking() {
	m_jugadores.lock();
	bool b = (jugadores.size() > 0);
	m_jugadores.unlock();
	return b;
}

/*bool Kinect::setMotorPosition(short position) {
	return SetNUIMotorPosition(motor, position);
}*/

const XnLabel * Kinect::getPixelesUsuario(XnUserID usuario) {
	m_generadores.lock();
	userG.GetUserPixels(usuario, *escena);
	m_generadores.unlock();
	return escena->Data();
}

int Kinect::startReconocedor(XnUserID jugador, Joint articulacion, GestoPatron *patron) {
	char* idRecBasico = new char(jugador + '_' + articulacion);
	ReconocedorBasico *recBasico = buscarReconocedorBasico(idRecBasico);

	m_reconocedores.lock();
	int i = 0;
	int idRec = -1;
	for(i; (i < reconocedores.size()) && (idRec == -1); i++) {
		Reconocedor *r = reconocedores.at(i);
		if(r->getIDJugador_Art() == idRecBasico && r->getGestoPatron() == patron)
			idRec = i;
	}
	if(i == reconocedores.size()) {
		idRec = i;
		XnSkeletonJointTransformation *art;
		m_generadores.lock();
		userG.GetSkeletonCap().GetSkeletonJoint(jugador, (XnSkeletonJoint)articulacion, *art);
		m_generadores.unlock();
		Reconocedor *nuevo = new Reconocedor(patron, idRecBasico, art, recBasico);
		reconocedores[i] = nuevo;
	}
	m_reconocedores.unlock();
	return idRec;
}

ReconocedorBasico * Kinect::buscarReconocedorBasico(char * idRecBasico) {
	m_reconocedoresBasicos.lock();
	if(reconocedoresBasicos.find(idRecBasico) == reconocedoresBasicos.end()){
		reconocedoresBasicos[idRecBasico] = new ReconocedorBasico(8, 70); //cambiar los parámetros por constantes
	}
	ReconocedorBasico * reconocedorBasicoAux;
	reconocedorBasicoAux = reconocedoresBasicos[idRecBasico];
	m_reconocedoresBasicos.unlock();
	return reconocedorBasicoAux;
}

void Kinect::updateReconocedoresBasicos() {
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

bool Kinect::isNuevoJugador(XnUserID &player) {
	if(nuevoJugadorID != -1) {
		player = nuevoJugadorID;
		nuevoJugadorID = -1;
		return true;
	}
	return false;
}

bool Kinect::isJugadorCalibrado(XnUserID &player) {
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

bool Kinect::isJugadorPerdido(XnUserID &player) {
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


//Agregar Listeners//

void Kinect::addListenerGesto(ListenerGesto *lg, int idRec) {
	m_reconocedores.lock();
	if (reconocedores.find(idRec) != reconocedores.end())
		reconocedores[idRec]->addListener(lg);
	m_reconocedores.unlock();
}

void Kinect::addListenerJugadorNuevo(ListenerJugadorNuevo *lnj) {
	m_listenersJugadorNuevo.lock();
	listenersJugadorNuevo.push_back(lnj);
	m_listenersJugadorNuevo.unlock();
}

void Kinect::addListenerJugadorPerdido(ListenerJugadorPerdido *ljp) {
	m_listenersJugadorPerdido.lock();
	listenersJugadorPerdido.push_back(ljp);
	m_listenersJugadorPerdido.unlock();
}

void Kinect::addListenerJugadorCalibrado(ListenerJugadorCalibrado *ljc) {
	m_listenersJugadorCalibrado.lock();
	listenersJugadorCalibrado.push_back(ljc);
	m_listenersJugadorCalibrado.unlock();
}


void Kinect::addListenerManoNueva(ListenerManoNueva *lmn) {
	m_listenersManoNueva.lock();
	listenersManoNueva.push_back(lmn);
	m_listenersManoNueva.unlock();
}

void Kinect::addListenerManoPerdida(ListenerManoPerdida *lmp) {
	m_listenersManoPerdida.lock();
	listenersManoPerdida.push_back(lmp);
	m_listenersManoPerdida.unlock();
}


//Notificadores//

void Kinect::notifyAllJugadorNuevo(XnUserID jugadorNuevo) {
	m_listenersJugadorNuevo.lock();
	for(int i = 0; i < listenersJugadorNuevo.size(); i++)
		listenersJugadorNuevo[i]->updateJugadorNuevo(jugadorNuevo);
	m_listenersJugadorNuevo.unlock();
}

void Kinect::notifyAllJugadorPerdido(XnUserID jugadorPerdido) {
	m_listenersJugadorPerdido.lock();
	for (int i = 0; i < listenersJugadorPerdido.size(); i++)
		listenersJugadorPerdido[i]->updateJugadorPerdido(jugadorPerdido);
	m_listenersJugadorPerdido.unlock();
}

void Kinect::notifyAllJugadorCalibrado( XnUserID jugadorCalibrado ) {
	m_listenersJugadorCalibrado.lock();
	for (int i = 0; i < listenersJugadorCalibrado.size(); i++)
		listenersJugadorCalibrado[i]->updateJugadorCalibrado(jugadorCalibrado);
	m_listenersJugadorCalibrado.unlock();
}

void Kinect::notifyAllManoNueva(XnUserID manoNueva) {
	m_listenersManoNueva.lock();
	for (int i = 0; i < listenersManoNueva.size(); i++)
		listenersManoNueva[i]->updateManoNueva(manoNueva);
	m_listenersManoNueva.unlock();
}

void Kinect::notifyAllManoPerdida(XnUserID manoPerdida) {
	m_listenersManoPerdida.lock();
	for (int i = 0; i < listenersManoPerdida.size(); i++)
		listenersManoPerdida[i]->updateManoPerdida(manoPerdida);
	m_listenersManoPerdida.unlock();
}

const Gesto * Kinect::getUltimoGesto(XnUserID player) {
	m_reconocedores.lock();
	std::map<int, Reconocedor *>::iterator it = reconocedores.begin();
	std::time_t time = 0; //variable donde almaceno el tiempo del gesto mas actual
	const Gesto * ultimoGesto, * gestoAux;
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

const Gesto * Kinect::getUltimoGesto(int idRec) {
	return reconocedores[idRec]->getUltimoGesto();
}

bool Kinect::isNuevaMano(XnUserID &mano) {
	if (nuevaManoID != -1){
		mano = nuevaManoID;
		m_manos.lock();
		manos[nuevaManoID] = nuevaMano;
		m_manos.unlock();
		nuevaManoID = -1;
		return true;
	}
	return false;
}

bool Kinect::isManoPerdida(XnUserID &mano) {
	if(manoPerdidaID != -1){
		mano = manoPerdidaID;
		m_manos.lock();
		manos.erase(manoPerdidaID);
		m_manos.unlock();
		manoPerdidaID = -1;
		return true;
	}
	return false;
}

const XnDepthPixel * Kinect::getMapaProfundidad() {
	return mapaProfundidad;
}

const XnUInt8 * Kinect::getMapaImagen() {
	return mapaImagen;
}

void Kinect::updateJugadores() {

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
			for (int i = 1; i <= 24; i++) { // 24 = cantidad de Kinect::Joint 
				XnSkeletonJointTransformation j;
				userG.GetSkeletonCap().GetSkeletonJoint(it->first, (XnSkeletonJoint)i, j);
				//		if(j.position.fConfidence > minConfidence)
				(it->second[i]) = j;
			}
	}
	m_jugadores.unlock();
}

void Kinect::updateManos() {
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
	if (manos.find(manoActualizada) != manos.end())	{
		manos[manoActualizada]->X = mano->X;
		manos[manoActualizada]->Y = mano->Y;
		manos[manoActualizada]->Z = mano->Z;
	}
	m_manos.unlock();
}
