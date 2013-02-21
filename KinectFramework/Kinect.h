#pragma once

#include "Sensor.h"
#include "OpenNI.h"
#include "NiTE.h"
#include "OpenNINiTEWrappers.h"

using namespace openni;

class Kinect : public Sensor
{
	

private:

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

	void stop();

public:

	Kinect();

	bool enableGenerator(TipoGenerador tipo);

	void disableGenerator(TipoGenerador tipo);

	const void * getMapaProfundidad();

	const void * getMapaImagen();

	const int getXRes( TipoSensor tipo );

	const int getYRes( TipoSensor tipo );

	const JugadorID * getPixelesUsuario( JugadorID usuario );


};