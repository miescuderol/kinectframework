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


/*bool Kinect::setMotorPosition(short position) {
	return SetNUIMotorPosition(motor, position);
}*/

const XnLabel * Kinect::getPixelesUsuario(XnUserID usuario) {
	m_generadores.lock();
	userG.GetUserPixels(usuario, *escena);
	m_generadores.unlock();
	return escena->Data();
}
