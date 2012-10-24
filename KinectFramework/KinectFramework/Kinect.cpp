#include "Kinect.h"

	xn::GestureGenerator gestureG; 
	xn::HandsGenerator handG; 
	xn::DepthGenerator depthG;
	xn::ImageGenerator imageG;
	xn::UserGenerator userG;
	
	XnUserID activeID = -1;
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
		activeID = nId;
		userG.GetPoseDetectionCap().StartPoseDetection("Psi", nId); 
	} 

	void XN_CALLBACK_TYPE User_LostUser(xn::UserGenerator& generator, 
					XnUserID nId, void* pCookie){

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
			activeID = nId;
			std::cout << "-----------------------------------------> " << activeID << std::endl;
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

void Kinect::notifyAllNuevoJugador( XnUserID jugadorNuevo ){
	if (listenersNuevoJugador.empty())	{
		std::cout << "no hay listeners jugador nuevo" << std::endl;
	}
	for(int i = 0; i < listenersNuevoJugador.size(); i++){
		listenersNuevoJugador[i]->update(jugadorNuevo);
	}
}



XnChar * getActiveGenerators(){
	return NULL;
}

void Kinect::update(){
	if (!checkStatusOK(contexto.WaitAnyUpdateAll(), "Wait and Update all lala"))
		return;

	mapaProfundidad = depthG.GetDepthMap();
	mapaImagen = imageG.GetImageMap();
	XnUserID newPlayer;
	if(isNewPlayer(newPlayer)) {
		std::cout << "nuevo jugador encontrado"<< newPlayer << std::endl;
		notifyAllNuevoJugador(newPlayer);
	}
	//TODO setear posiciones nuevas a los reconocedores basicos

	//TODO agregar updates de los gestos

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

bool Kinect::isTrackingPlayer(XnUserID player){
	return userG.IsGenerating() && userG.IsCapabilitySupported(XN_CAPABILITY_SKELETON) && userG.GetSkeletonCap().IsTracking(player);
}

bool Kinect::isTracking() {
	return userG.IsGenerating() && userG.IsCapabilitySupported(XN_CAPABILITY_SKELETON) && userG.GetSkeletonCap().IsTracking(activeID);
}

/*bool Kinect::setMotorPosition(short position) {
	return SetNUIMotorPosition(motor, position);
}*/

const XnLabel * Kinect::getPixelesUsuario() {
	userG.GetUserPixels(activeID, *escena);
	return escena->Data();
}

DepthGenerator * Kinect::getGenProfundidad() {
	return &depthG;
}

XnUserID Kinect::getIDActivo(){
	return activeID;
}

bool Kinect::isNewPlayer(XnUserID& player) {
	if(activeID != -1) {
		player = activeID;
		activeID = -1;
		users[player] = new XnSkeletonJointTransformation[25]; // el [0] no se usa
		//cout << "---------------------------------posicion &: " << &usuarios[player][Kinect::LEFT_HAND].position.position.X << endl;
		return true;
	}
	return false;
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

Gesto *Kinect::isGesto(int idRec){
	return reconocedores.at(idRec)->getUltimoGesto();
}

void Kinect::addListenerReconocedor(ListenerReconocedor *lr, int idRec){

}

void Kinect::addListenerNuevoJugador(ListenerNuevoJugador *lnj){
	listenersNuevoJugador.push_back(lnj);
}

void Kinect::addListenerJugadorPerdido(ListenerJugadorPerdido *ljp){

}

void Kinect::addListenerJugadorCalibrado(ListenerJugadorCalibrado *ljc){

}

ReconocedorBasico * Kinect::buscarReconocedorBasico(char * idRecBasico) {
	
	if(reconocedoresBasicos.find(idRecBasico) == reconocedoresBasicos.end()){
		reconocedoresBasicos[idRecBasico] = new ReconocedorBasico(8, 70); //cambiar los parámetros por constantes
	}
	return reconocedoresBasicos[idRecBasico];
}
