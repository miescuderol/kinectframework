#pragma once

#include "XnCppWrapper.h"
#include <iostream>
#include <map>
#include <list>
#include "Reconocedor.h"
#include "XnTypes.h"
#include "ListenerJugadorCalibrado.h"
#include "ListenerJugadorPerdido.h"
#include "ListenerMovimiento.h"
#include "ListenerNuevoJugador.h"
#include "ListenerReconocedor.h"

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
		GESTURE_GENERATOR,
		ALL_GENERATORS
	};

private:
	XnStatus estado;
	Context contexto;

	const XnDepthPixel * mapaProfundidad;
	const XnUInt8 * mapaImagen;
	DepthMetaData metaDataProfundidad;
	SceneMetaData * escena;
	
	//Lista de reconocedores
	std::map<int, Reconocedor* > reconocedores;
	//Map de reconocedores básicos asociados a una articulación
	std::map<char*, ReconocedorBasico *> reconocedoresBasicos;
	std::list<ListenerJugadorCalibrado *> listenersJugadorCalibrado;
	std::list<ListenerJugadorPerdido *> listenersJugadorPerdido;
	std::list<ListenerNuevoJugador *> listenersNuevoJugador;
	std::list<ListenerReconocedor *> listenersReconocedor;

	bool checkStatusOK(const XnStatus estado, char* entorno);


public:

	


	Kinect(void);
	~Kinect(void);

	void setup();
	void update();
	bool enableGenerator(GeneratorType generator);
	bool disableGenerator(GeneratorType generator);
	bool setMotorPosition(short position);
	XnChar * getActiveGenerators();

	inline const XnDepthPixel * getMapaProfundidad() { return mapaProfundidad; }
	inline const XnUInt8 * getMapaImagen() { return mapaImagen; }
	const int getXRes();
	const int getYRes();
	const XnPoint3D * getMano();
	bool getArticulaciones(Joint * articulaciones, XnSkeletonJointTransformation * jPosiciones, XnUInt8 nArticulaciones);
	bool isTrackingPlayer(XnUserID jugador);
	bool isTracking();
	void setID(XnUserID nId);
	XnUserID getIDActivo();
	const XnLabel * getPixelesUsuario();
	DepthGenerator * getGenProfundidad();

	//startReconocedor crea un nuevo reconocedor con las características pasadas por parámetro y devuelve el id del mismo.
	//en caso de haber un reconocedor similar devuelve el id correspondiente en lugar de crear uno nuevo
	int startReconocedor(XnUserID jugador, Joint articulacion, GestoPatron *patron); 

	ReconocedorBasico *buscarReconocedorBasico(char * idRecBasico);

	Gesto *isGesto(int idRec); //si cada reconocedor tiene un unico gesto, SI NO deberia devolver el gesto encontrado
	
	//métodos para agregar listeners
	void addListenerReconocedor(ListenerReconocedor *lr, int idRec); 
	void addListenerNuevoJugador(ListenerNuevoJugador *lnj); 
	void addListenerJugadorPerdido(ListenerJugadorPerdido *ljp);
	void addListenerJugadorCalibrado(ListenerJugadorCalibrado *ljc);


	Gesto * getUltimoGesto(XnUserID player);






};

