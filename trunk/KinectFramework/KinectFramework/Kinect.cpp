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
	XnPoint3D * hand;
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
		hand = new XnPoint3D;
		hand->X = pPosition->X;
		hand->Y = pPosition->Y;
		hand->Z = pPosition->Z;
	} 
	void XN_CALLBACK_TYPE Hand_Update(xn::HandsGenerator& generator, 
				XnUserID nId, const XnPoint3D* pPosition, 
				XnFloat fTime, void* pCookie){ 
		if (hand) {
			hand->X = pPosition->X;
			hand->Y = pPosition->Y;
			hand->Z = pPosition->Z;
		}
	}

	void XN_CALLBACK_TYPE Hand_Destroy(xn::HandsGenerator& generator, 
				 XnUserID nId, XnFloat fTime, 
				 void* pCookie){ 
	  printf("Lost Hand: %d\n", nId); 
	  recognizedGesture = NULL;
	  delete hand;
	  hand = NULL;
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

Kinect::Kinect(void){

}


Kinect::~Kinect(void){

}

void Kinect::setup(){

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

bool Kinect::enableGenerator(GeneratorType generator){
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

bool Kinect::disableGenerator(GeneratorType generator){
	return false;
}

XnChar * getActiveGenerators(){
	return NULL;
}


void Kinect::update(){
	if (!checkStatusOK(contexto.WaitAnyUpdateAll(), "Wait and Update all lala"))
		return;

	mapaProfundidad = depthG.GetDepthMap();
	mapaImagen = imageG.GetImageMap();

	XnUserID jugadorNuevo;
	if(isNuevoJugador(jugadorNuevo)) {
		std::cout << "nuevo jugador encontrado"<< jugadorNuevo << std::endl;
		notifyAllNuevoJugador(jugadorNuevo);
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
	//TODO setear posiciones nuevas a los reconocedores basicos
	updateReconocedoresBasicos();

	//notifico a todos los listeners de reconocedores
	


}

bool Kinect::checkStatusOK(const XnStatus status, char* entorno){
	if (status != XN_STATUS_OK) {
		std::cout << xnGetStatusString(status) << " " << entorno << std::endl;
		return false;
	}
	return true;
}

const int Kinect::getXRes(){
	DepthMetaData m;
	depthG.GetMetaData(m);
	return m.XRes();
}

const int Kinect::getYRes(){
	DepthMetaData m;
	depthG.GetMetaData(m);
	return m.YRes();
}

const XnPoint3D * Kinect::getMano(){
	return hand;
}

bool Kinect::getArticulaciones(Joint * joints, XnSkeletonJointTransformation * jTransformations, XnUInt8 nJoints){
	if (!isTrackingPlayer(activeID)) return false;	

	for (int i = 0; i < nJoints; i++)
		userG.GetSkeletonCap().GetSkeletonJoint(activeID, (XnSkeletonJoint)joints[i], jTransformations[i]);

	return true;
}

XnSkeletonJointTransformation * Kinect::getArticulaciones(XnUserID jugador) {
	if (jugadores.find(jugador) != jugadores.end())
		return jugadores[jugador];
	return NULL;
}

bool Kinect::isTrackingPlayer(XnUserID player){
	return userG.IsGenerating() && userG.IsCapabilitySupported(XN_CAPABILITY_SKELETON) && userG.GetSkeletonCap().IsTracking(player);
}

bool Kinect::isTracking() {
	return userG.IsGenerating() && userG.IsCapabilitySupported(XN_CAPABILITY_SKELETON) && userG.GetSkeletonCap().IsTracking(activeID);
}

/*bool Kinect::setMotorPosition(short position) {
	return SetNUIMotorPosition(motor, position);
}*/

const XnLabel * Kinect::getPixelesUsuario(XnUserID usuario) {
	userG.GetUserPixels(usuario, *escena);
	return escena->Data();
}

DepthGenerator * Kinect::getGenProfundidad() {
	return &depthG;
}

XnUserID Kinect::getIDActivo(){
	return activeID;
}



int Kinect::startReconocedor(XnUserID jugador, Joint articulacion, GestoPatron *patron){
	char* idRecBasico = new char(jugador + '_' + articulacion);
	ReconocedorBasico *recBasico = buscarReconocedorBasico(idRecBasico);

	int i = 0;
	for(i; i < reconocedores.size(); i++){
		Reconocedor *r = reconocedores.at(i);
		if(r->getIDJugadorArt() == idRecBasico && r->getGestoPatron() == patron)
			return i;
	}
	if(i == reconocedores.size()){
		XnSkeletonJointTransformation *art;
		userG.GetSkeletonCap().GetSkeletonJoint(jugador, (XnSkeletonJoint)articulacion, *art);
		Reconocedor *nuevo = new Reconocedor(patron, idRecBasico, art, recBasico);
		reconocedores[i] = nuevo;
	}
	return i;
}

ReconocedorBasico * Kinect::buscarReconocedorBasico(char * idRecBasico) {

	if(reconocedoresBasicos.find(idRecBasico) == reconocedoresBasicos.end()){
		reconocedoresBasicos[idRecBasico] = new ReconocedorBasico(8, 70); //cambiar los parámetros por constantes
	}
	return reconocedoresBasicos[idRecBasico];
}

void Kinect::updateReconocedoresBasicos(){
	std::map<char*, ReconocedorBasico*>::iterator it = reconocedoresBasicos.begin();
	for(it; it != reconocedoresBasicos.end(); it++){
		std::string clave = it->first;
		XnUserID us = atoi(clave.substr(0, clave.find_first_of('_')).data());
		Joint joint = (Joint)atoi(clave.substr(clave.find_first_of('_')+1).data());
		XnSkeletonJointTransformation * joints = jugadores[us];
		it->second->setNewPosition(joints[joint].position.position.X, joints[joint].position.position.Y, joints[joint].position.position.Z);
	}
}

Gesto *Kinect::isGesto(int idRec){
	return reconocedores.at(idRec)->getUltimoGesto();
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
		jugadores[player] = new XnSkeletonJointTransformation[25]; // el [0] no se usa
		return true;
	}
	return false;
}

bool Kinect::isJugadorPerdido(XnUserID &player) {
	if(jugadorPerdidoID != -1) {
		player = jugadorPerdidoID;
		jugadorPerdidoID = -1;
		jugadores.erase(player);
		return true;
	}
	return false;
}


//Agregar Listeners//

void Kinect::addListenerReconocedor(ListenerReconocedor *lr, int idRec){
	listenersReconocedor[idRec].push_back(lr);
}

void Kinect::addListenerNuevoJugador(ListenerNuevoJugador *lnj){
	listenersNuevoJugador.push_back(lnj);
}

void Kinect::addListenerJugadorPerdido(ListenerJugadorPerdido *ljp){
	listenersJugadorPerdido.push_back(ljp);
}

void Kinect::addListenerJugadorCalibrado(ListenerJugadorCalibrado *ljc){
	listenersJugadorCalibrado.push_back(ljc);
}


//Notificadores//

void Kinect::notifyAllReconocedor(int idRec){
	std::vector<ListenerReconocedor*> listeners = listenersReconocedor[idRec];
	for (int i = 0; i < listeners.size(); i++){
		listeners[i]->updateReconocedor(reconocedores[idRec]);
	}
}

void Kinect::notifyAllNuevoJugador( XnUserID jugadorNuevo ){
	if (listenersNuevoJugador.empty())	{
		std::cout << "no hay listeners jugador nuevo" << std::endl;
	}
	for(int i = 0; i < listenersNuevoJugador.size(); i++){
		listenersNuevoJugador[i]->updateNuevoJugador(jugadorNuevo);
	}
}

void Kinect::notifyAllJugadorPerdido(XnUserID jugadorPerdido){
	for (int i = 0; i < listenersJugadorPerdido.size(); i++)
		listenersJugadorPerdido[i]->updateJugadorPerdido(jugadorPerdido);
}

void Kinect::notifyAllJugadorCalibrado( XnUserID jugadorCalibrado ){
	for (int i = 0; i < listenersJugadorCalibrado.size(); i++)
		listenersJugadorCalibrado[i]->updateJugadorCalibrado(jugadorCalibrado);
}


