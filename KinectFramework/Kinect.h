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
	//Context contexto;

	/*const XnDepthPixel * mapaProfundidad;
	const XnUInt8 * mapaImagen;
	DepthMetaData metaDataProfundidad;
	SceneMetaData * escena;*/

	JugadorID _jugadorNuevo, _jugadorPerdido;
	const nite::HandData * _manoNueva, * _manoPerdida, * _manoActualizada;
	std::vector<JugadorID> jugadoresTrackeados;

	// Semáforos
	boost::mutex m_mapas;

	Device dispositivo;
	VideoStream camaraProfundidad, camaraImagen;
	nite::UserTracker userTracker;
	nite::HandTracker handTracker;

	VideoFrameRef * mapaProfundidad, * mapaImagen;
	nite::UserTrackerFrameRef * mapaUsuarios;
	nite::HandTrackerFrameRef * mapaManos;

	void updateArticulacionesJugadores();

	JugadorID jugadorNuevo();

	JugadorID jugadorCalibrado(Esqueleto *& esqueleto);

	JugadorID jugadorPerdido();

	JugadorID manoNueva( Punto3f *& mano );

	JugadorID manoActualizada( Punto3f *& mano );

	JugadorID manoPerdida();

	void setup();

	void update();

public:

	bool enableGenerator(TipoGenerador tipo);

	void disableGenerator(TipoGenerador tipo);

	const void * getMapaProfundidad();

	const void * getMapaImagen();

	const int getXRes( TipoSensor tipo );

	const int getYRes( TipoSensor tipo );

	const JugadorID * getPixelesUsuario( JugadorID usuario );

};