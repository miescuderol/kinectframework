#pragma once

#include "XnCppWrapper.h"
#include "XnTypes.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "Subsistema.h"
#include "Reconocedor.h"
#include "ListenerJugadorCalibrado.h"
#include "ListenerJugadorPerdido.h"
#include "ListenerMovimiento.h"
#include "ListenerNuevoJugador.h"
#include "ListenerReconocedor.h"
#include "ListenerManoNueva.h"
#include "ListenerManoPerdida.h"
#include <boost/chrono/system_clocks.hpp>

using namespace xn;

class Kinect
{
public:
	
	enum Joint{
		HEAD			= 1,
		NECK			= 2,
		TORSO			= 3,
		WAIST			= 4,

		LEFT_COLLAR		= 5,
		LEFT_SHOULDER	= 6,
		LEFT_ELBOW		= 7,
		LEFT_WRIST		= 8,
		LEFT_HAND		= 9,
		LEFT_FINGERTIP	=10,

		RIGHT_COLLAR	=11,
		RIGHT_SHOULDER	=12,
		RIGHT_ELBOW		=13,
		RIGHT_WRIST		=14,
		RIGHT_HAND		=15,
		RIGHT_FINGERTIP	=16,

		LEFT_HIP		=17,
		LEFT_KNEE		=18,
		LEFT_ANKLE		=19,
		LEFT_FOOT		=20,

		RIGHT_HIP		=21,
		RIGHT_KNEE		=22,
		RIGHT_ANKLE		=23,
		RIGHT_FOOT		=24	
	};
	 enum GeneratorType {
		IMAGE_GENERATOR, 
		DEPTH_GENERATOR, 
		USER_GENERATOR, 
		HAND_GENERATOR,
		GESTURE_GENERATOR
	};

private:
	XnStatus estado;
	Context contexto;

	const XnDepthPixel * mapaProfundidad;
	const XnUInt8 * mapaImagen;
	DepthMetaData metaDataProfundidad;
	SceneMetaData * escena;

	bool started;

	// Semaforos
	boost::mutex m_manos;
	boost::mutex m_jugadores;
	boost::mutex m_generadores;
	boost::mutex m_listenersJugadorCalibrado;
	boost::mutex m_listenersJugadorPerdido;
	boost::mutex m_listenersJugadorNuevo;
	boost::mutex m_listenersManoNueva;
	boost::mutex m_listenersManoPerdida;
	boost::mutex m_reconocedores;
	boost::mutex m_reconocedoresBasicos;

	std::vector<GeneratorType> generadoresActivos;
	
	/** Map que asocia un arreglo de articulaciones (XnSkeletonJointTransformation[25]) a un id de jugador */
	std::map<XnUserID, XnSkeletonJointTransformation*> jugadores;
	
	/** Map que asocia una mano (XnPoint3D*) a un id de jugador. */
	std::map<XnUserID, XnPoint3D*> manos; 

	/** Map de reconocedores asociados a un idRec */
	std::map<int, Reconocedor* > reconocedores;

	/** Map de reconocedores básicos asociados a una articulación.*/
	std::map<char *, ReconocedorBasico *> reconocedoresBasicos;

	/** Vector de listeners de jugadores calibrados.*/
	std::vector<ListenerJugadorCalibrado *> listenersJugadorCalibrado;
	/** Vector de listeners de jugadores perdidos.*/
	std::vector<ListenerJugadorPerdido *> listenersJugadorPerdido;
	/** Vector de listeners de jugadores nuevos.*/
	std::vector<ListenerJugadorNuevo *> listenersJugadorNuevo;
	/** Vector de listeners de manos nuevas.*/
	std::vector<ListenerManoNueva *> listenersManoNueva;
	/** Vector de listeners de manos perdidas.*/
	std::vector<ListenerManoPerdida *> listenersManoPerdida;

	boost::thread threadKinect;

	bool checkStatusOK(const XnStatus estado, char* entorno);

	void notifyAllJugadorNuevo(XnUserID jugadorNuevo);
	void notifyAllJugadorPerdido(XnUserID jugadorPerdido);
	void notifyAllJugadorCalibrado(XnUserID jugadorCalibrado);
	void notifyAllManoNueva(XnUserID manoNueva);
	void notifyAllManoPerdida(XnUserID manoPerdida);

	ReconocedorBasico * buscarReconocedorBasico(char * idRecBasico);

	void updateReconocedoresBasicos();	/**< Setea posiciones nuevas a los reconocedores basicos. */
	void updateJugadores();
	void updateManos();

	void setup();
	void update();
	void run();

public:

	Kinect(void);
	~Kinect(void);

	void start();
	/**< Inicia la ejecución del componente Kinect en otro thread */

	void shutdown();
	/**< Termina la ejecución del componente Kinect */

	bool enableGenerator(GeneratorType generator);
	void disableGenerator(GeneratorType generator);
	bool setMotorPosition(short position);
	std::vector<GeneratorType> getActiveGenerators();

	const XnDepthPixel * getMapaProfundidad();
	const XnUInt8 * getMapaImagen();
	const int getXRes();
	const int getYRes();
	const XnPoint3D * getMano(XnUserID jugador);
	const XnSkeletonJointTransformation * getArticulaciones(XnUserID jugador);
	bool isTrackingPlayer(XnUserID jugador);
	bool isTracking();
	const XnLabel * getPixelesUsuario(XnUserID usuario);

	//startReconocedor crea un nuevo reconocedor con las características pasadas por parámetro y devuelve el id del mismo.
	//en caso de haber un reconocedor similar devuelve el id correspondiente en lugar de crear uno nuevo
	int startReconocedor(XnUserID jugador, Joint articulacion, GestoPatron *patron);

	bool isNuevoJugador(XnUserID &jugador);
	bool isJugadorCalibrado(XnUserID &jugador);
	bool isJugadorPerdido(XnUserID &jugador);

	bool isNuevaMano(XnUserID &mano);
	bool isManoPerdida(XnUserID &mano);

	bool isStarted();

	//métodos para agregar listeners
	void addListenerGesto(ListenerGesto *lg, int idRec);
	void addListenerJugadorNuevo(ListenerJugadorNuevo *lnj);
	void addListenerJugadorPerdido(ListenerJugadorPerdido *ljp);
	void addListenerJugadorCalibrado(ListenerJugadorCalibrado *ljc);
	void addListenerManoNueva(ListenerManoNueva *lmn);
	void addListenerManoPerdida(ListenerManoPerdida *lmp);


	const Gesto * getUltimoGesto(XnUserID player);
	const Gesto * getUltimoGesto(int idRec);

};