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

	/*

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
*/

/*bool Kinect::setMotorPosition(short position) {
	return SetNUIMotorPosition(motor, position);
}*/
	/*
const XnLabel * Kinect::getPixelesUsuario(XnUserID usuario) {
	m_generadores.lock();
	userG.GetUserPixels(usuario, *escena);
	m_generadores.unlock();
	return escena->Data();
}
*/

	void Kinect::updateArticulacionesJugador( XnUserID jugador )
	{
		
		throw std::exception("The method or operation is not implemented.");
	}

	XnUserID Kinect::jugadorNuevo()
	{
		XnUserID aux = jugadorNuevo;
		jugadorNuevo = -1;
		return aux;
	}

	XnUserID Kinect::jugadorCalibrado(Esqueleto *& esqueleto)
	{
		XnUserID jugadorCalibrado = -1;
		for (int i = 0; i < jugadoresTrackeados.size(); i++)
			if (jugadores.find(jugadoresTrackeados[i]) == jugadoresTrackeados.end())
				jugadorCalibrado = jugadoresTrackeados[i];

		jugadoresTrackeados.clear();

		if (jugadorCalibrado != -1)
			esqueleto = new NiTEEsqueleto(mapaUsuarios->getUserById(jugadorCalibrado)->getSkeleton());
		else 
			esqueleto = NULL;

		return jugadorCalibrado;
	}

	XnUserID Kinect::jugadorPerdido()
	{
		XnUserID aux = jugadorPerdido;
		jugadorPerdido = -1;
		return aux;
	}

	XnUserID Kinect::manoNueva( Punto3f *& manoNueva )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	XnUserID Kinect::manoActualizada( Punto3f *& manoActualizada )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	XnUserID Kinect::manoPerdida()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void Kinect::setup()
	{
		OpenNI::initialize();
		nite::NiTE::initialize();

		dispositivo.open(openni::ANY_DEVICE);

		mapaImagen = new VideoFrameRef();
		mapaProfundidad = new VideoFrameRef();
		mapaUsuarios = new nite::UserTrackerFrameRef();
		mapaManos = new nite::HandTrackerFrameRef();
	}

	void Kinect::update()
	{
		// Actualiza frames de profundidad e imagen
		m_mapas.lock();
		if (camaraProfundidad.isValid())
			camaraProfundidad.readFrame(mapaProfundidad);
		if (camaraImagen.isValid())
			camaraImagen.readFrame(mapaImagen);
		if (userTracker.isValid()) {
			userTracker.readFrame(mapaUsuarios);

			const nite::Array<nite::UserData>& usuarios = mapaUsuarios->getUsers();
			for (int i = 0; i < usuarios.getSize(); ++i) {
				const nite::UserData& usuario = usuarios[i];
				if (usuario.isNew()) {
					jugadorNuevo = usuario.getId();
					userTracker.startSkeletonTracking(jugadorNuevo);
				} else if (usuario.isLost()) {
					jugadorPerdido = usuario.getId();
					userTracker.stopSkeletonTracking(jugadorPerdido);
				} else if (usuario.getSkeleton().getState() == nite::SKELETON_TRACKED) {
					jugadoresTrackeados.push_back(usuario.getId());
				}
			}

		}
		if (handTracker.isValid())
			handTracker.readFrame(mapaManos);
		m_mapas.unlock();

		updateJugadores();

		updateReconocedoresBasicos();

		updateManos();
	}

	bool Kinect::enableGenerator( GeneratorType tipo )
	{
		switch (tipo) {
		case Sensor::DEPTH_GENERATOR:
			if(dispositivo.isValid() && dispositivo.hasSensor(SENSOR_DEPTH)) {
				camaraProfundidad.create(dispositivo, SENSOR_DEPTH);
				if (camaraProfundidad.start() == STATUS_OK)
					return true;
				else {
					camaraProfundidad.destroy();
					return false;
				}
			} else
				return false;
			break;
		case Sensor::IMAGE_GENERATOR:
			if(dispositivo.isValid() && dispositivo.hasSensor(SENSOR_COLOR)) {
				camaraImagen.create(dispositivo, SENSOR_COLOR);
				if (camaraImagen.start() == STATUS_OK)
					return true;
				else {
					camaraImagen.destroy();
					return false;
				}
			} else
				return false;
			break;
		case Sensor::USER_GENERATOR:
			if (userTracker.create(&dispositivo) == nite::STATUS_OK)
				return true;
			else return false;
			break;
		case Sensor::HAND_GENERATOR:
			if (handTracker.create(&dispositivo) == nite::STATUS_OK)
				return true;
			else return false;
			break;
		default:
			return false;
		}
		
	}

	void Kinect::disableGenerator( GeneratorType tipo )
	{
		switch (tipo) {
		case Sensor::DEPTH_GENERATOR:
			camaraProfundidad.stop();
			camaraProfundidad.destroy();
			break;
		case Sensor::IMAGE_GENERATOR:
			camaraImagen.stop();
			camaraImagen.destroy();
			break;
		case Sensor::USER_GENERATOR:
			userTracker.destroy();
			break;
		case Sensor::HAND_GENERATOR:
			handTracker.destroy();
			break;
		default:
			return false;
		}
	}

	const VideoFrameRef * Kinect::getMapaProfundidad()
	{
		m_mapas.lock();
		VideoFrameRef * aux = new VideoFrameRef();
		*aux = *mapaProfundidad;
		m_mapas.unlock();
		return aux;
	}

	const VideoFrameRef * Kinect::getMapaImagen()
	{
		m_mapas.lock();
		VideoFrameRef * aux = new VideoFrameRef();
		*aux = *mapaImagen;
		m_mapas.unlock();
		return aux;
	}

	const int Kinect::getXRes( SensorType tipo )
	{
		if (camaras.find(tipo) != camaras.end())
			return camaras[tipo]->getVideoMode().getResolutionX();
		else
			return -1;
	}

	const int Kinect::getYRes( SensorType tipo )
	{
		if (camaras.find(tipo) != camaras.end())
			return camaras[tipo]->getVideoMode().getResolutionY();
		else
			return -1;
	}

	const XnPoint3D * Kinect::getMano( XnUserID jugador )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	const XnSkeletonJointTransformation * Kinect::getArticulaciones( XnUserID jugador )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	bool Kinect::isTrackingPlayer( XnUserID jugador )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	bool Kinect::isTracking()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	const XnLabel * Kinect::getPixelesUsuario( XnUserID usuario )
	{
		throw std::exception("The method or operation is not implemented.");
	}
