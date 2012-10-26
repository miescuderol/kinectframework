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

using namespace xn;

class Kinect : public Subsistema
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
	
	//Map que asocia un arreglo de articulaciones (XnSkeletonJointTransformation[25]) a un id de jugador
	std::map<XnUserID, XnSkeletonJointTransformation*> jugadores;

	//Map de reconocedores asociados a un idRec
	std::map<int, Reconocedor* > reconocedores;

	//Map de reconocedores básicos asociados a una articulación
	std::map<char *, ReconocedorBasico *> reconocedoresBasicos;

	//Vectores de listeners
	std::vector<ListenerJugadorCalibrado *> listenersJugadorCalibrado;
	std::vector<ListenerJugadorPerdido *> listenersJugadorPerdido;
	std::vector<ListenerNuevoJugador *> listenersNuevoJugador;

	//Map que asocia un vector de listeners al idRec correspondiente 
	std::map<int, std::vector<ListenerReconocedor *> > listenersReconocedor;

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
	XnSkeletonJointTransformation * getArticulaciones(XnUserID jugador);
	bool isTrackingPlayer(XnUserID jugador);
	bool isTracking();
	void setID(XnUserID nId);
	XnUserID getIDActivo();
	const XnLabel * getPixelesUsuario(XnUserID usuario);
	DepthGenerator * getGenProfundidad();

	//startReconocedor crea un nuevo reconocedor con las características pasadas por parámetro y devuelve el id del mismo.
	//en caso de haber un reconocedor similar devuelve el id correspondiente en lugar de crear uno nuevo
	int startReconocedor(XnUserID jugador, Joint articulacion, GestoPatron *patron); 

	ReconocedorBasico *buscarReconocedorBasico(char * idRecBasico);

	void updateReconocedoresBasicos();


	Gesto *isGesto(int idRec); //si cada reconocedor tiene un unico gesto, SI NO deberia devolver el gesto encontrado
	bool isNuevoJugador(XnUserID &jugador); 
	bool isJugadorCalibrado(XnUserID &jugador);
	bool isJugadorPerdido(XnUserID &jugador);

	//métodos para agregar listeners
	void addListenerReconocedor(ListenerReconocedor *lr, int idRec); 
	void addListenerNuevoJugador(ListenerNuevoJugador *lnj); 
	void addListenerJugadorPerdido(ListenerJugadorPerdido *ljp);
	void addListenerJugadorCalibrado(ListenerJugadorCalibrado *ljc);

	void notifyAllReconocedor(int idRec);
	void notifyAllNuevoJugador(XnUserID jugadorNuevo);
	void notifyAllJugadorPerdido(XnUserID jugadorPerdido);
	void notifyAllJugadorCalibrado(XnUserID jugadorCalibrado);


	Gesto * getUltimoGesto(XnUserID player);

};

