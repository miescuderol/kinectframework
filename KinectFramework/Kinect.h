#pragma once

#include "Sensor.h"
#include "OpenNI.h"
#include "NiTE.h"
#include "OpenNINiTEWrappers.h"

using namespace openni;

class Kinect : public Sensor
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
	

private:
	XnStatus estado;
	//Context contexto;

	/*const XnDepthPixel * mapaProfundidad;
	const XnUInt8 * mapaImagen;
	DepthMetaData metaDataProfundidad;
	SceneMetaData * escena;*/

	XnUserID jugadorNuevo, jugadorPerdido;
	std::vector<XnUserID> jugadoresTrackeados;

	// Semáforos
	boost::mutex m_mapas;

	Device dispositivo;
	VideoStream camaraProfundidad, camaraImagen;
	nite::UserTracker userTracker;
	nite::HandTracker handTracker;

	VideoFrameRef * mapaProfundidad, * mapaImagen;
	nite::UserTrackerFrameRef * mapaUsuarios;
	nite::HandTrackerFrameRef * mapaManos;

	void updateArticulacionesJugador( XnUserID jugador );

	XnUserID jugadorNuevo();

	XnUserID jugadorCalibrado(Esqueleto *& esqueleto);

	XnUserID jugadorPerdido();

	XnUserID manoNueva( Punto3f *& manoNueva );

	XnUserID manoActualizada( Punto3f *& manoActualizada );

	XnUserID manoPerdida();

	void setup();

	void update();

public:

	bool enableGenerator(GeneratorType tipo);

	void disableGenerator(GeneratorType tipo);

	const VideoFrameRef * getMapaProfundidad();

	const VideoFrameRef * getMapaImagen();

	const int getXRes( SensorType tipo );

	const int getYRes( SensorType tipo );

	const XnPoint3D * getMano( XnUserID jugador );

	const XnSkeletonJointTransformation * getArticulaciones( XnUserID jugador );

	bool isTrackingPlayer( XnUserID jugador );

	bool isTracking();

	const XnLabel * getPixelesUsuario( XnUserID usuario );

};