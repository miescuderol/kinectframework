#pragma once

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
#include "Wrappers.h"

class Sensor
{
	//!\todo Agregar un metodo para controlar el estado del dispositivo

public:

	enum TipoGenerador {
		IMAGE_GENERATOR, 
		DEPTH_GENERATOR, 
		USER_GENERATOR, 
		HAND_GENERATOR
	};

	enum TipoSensor {
		DEPTH_SENSOR,
		COLOR_SENSOR
	};

private:

	bool started;

	boost::thread threadKinect;

	// Semaforos
	boost::mutex m_manos;
	boost::mutex m_jugadores;
	boost::mutex m_listenersJugadorCalibrado;
	boost::mutex m_listenersJugadorPerdido;
	boost::mutex m_listenersJugadorNuevo;
	boost::mutex m_listenersManoNueva;
	boost::mutex m_listenersManoPerdida;
	boost::mutex m_reconocedores;
	boost::mutex m_reconocedoresBasicos;

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

protected:

	std::vector<TipoGenerador> generadoresActivos;

	/** Map que asocia un arreglo de articulaciones (XnSkeletonJointTransformation[25]) a un id de jugador */
	std::map<JugadorID, Esqueleto*> jugadores;

	/** Map que asocia una mano (XnPoint3D*) a un id de jugador. */
	std::map<JugadorID, Punto3f*> manos; 

	void notifyAllJugadorNuevo(JugadorID jugadorNuevo);
	void notifyAllJugadorPerdido(JugadorID jugadorPerdido);
	void notifyAllJugadorCalibrado(JugadorID jugadorCalibrado);
	void notifyAllManoNueva(JugadorID manoNueva);
	void notifyAllManoPerdida(JugadorID manoPerdida);

	ReconocedorBasico * buscarReconocedorBasico(char * idRecBasico);

	void updateReconocedoresBasicos();	/**< Setea posiciones nuevas a los reconocedores basicos. */
	void updateJugadores();
	void updateManos();
	virtual void updateArticulacionesJugadores() = 0;

	virtual JugadorID jugadorNuevo() = 0; /**< Devuelve el ID del jugador nuevo. De no existir, -1. */
	virtual JugadorID jugadorCalibrado(Esqueleto *& esqueleto) = 0; /**< Devuelve el ID del jugador calibrado. De no existir, -1. \param esqueleto El esqueleto del jugador calibrado */
	virtual JugadorID jugadorPerdido() = 0; /**< Devuelve el ID del jugador perdido. De no existir, -1. */
	virtual JugadorID manoNueva(Punto3f *& mano) = 0; /**< Devuelve el ID de la nueva mano. De no existir, -1. \param manoNueva Nueva mano encontrada */
	virtual JugadorID manoActualizada(Punto3f *& mano) = 0; /**< Devuelve el ID de la nueva mano. De no existir, -1. \param manoNueva Nueva mano encontrada */
	virtual JugadorID manoPerdida() = 0; /**< Devuelve el ID de la nueva mano. De no existir, -1. */
	

	virtual void setup() = 0;
	virtual	void update() = 0;
	void run();


public:

	Sensor();

	void start();
	/**< Inicia la ejecución del componente DepthDevice en otro thread */

	void shutdown();
	/**< Termina la ejecución del componente DepthDevice */

	virtual bool enableGenerator(TipoGenerador tipo) = 0;
	virtual void disableGenerator(TipoGenerador tipo) = 0;
	std::vector<TipoGenerador> getActiveGenerators();

	virtual const void * getMapaProfundidad() = 0;
	virtual const void * getMapaImagen() = 0;
	virtual const int getXRes( TipoSensor tipo ) = 0;
	virtual const int getYRes( TipoSensor tipo ) = 0;
	virtual const JugadorID * getPixelesUsuario(JugadorID usuario) = 0;
	const Punto3f * getMano(JugadorID jugador);
	const Esqueleto * getArticulaciones(JugadorID jugador);
	bool isTrackingPlayer(JugadorID jugador);
	bool isTracking();

	//startReconocedor crea un nuevo reconocedor con las características pasadas por parámetro y devuelve el id del mismo.
	//en caso de haber un reconocedor similar devuelve el id correspondiente en lugar de crear uno nuevo
	int startReconocedor(JugadorID jugador, int articulacion, GestoPatron *patron);
	//!\todo stopReconocedor(...);


	bool isNuevoJugador(JugadorID &jugador);
	bool isJugadorCalibrado(JugadorID &jugador);
	bool isJugadorPerdido(JugadorID &jugador);

	bool isNuevaMano(JugadorID &mano);
	bool isManoPerdida(JugadorID &mano);

	bool isStarted();
	bool isActive( TipoGenerador tipo );

	//métodos para agregar listeners
	void addListenerGesto(ListenerGesto *lg, int idRec);
	void addListenerJugadorNuevo(ListenerJugadorNuevo *lnj);
	void addListenerJugadorPerdido(ListenerJugadorPerdido *ljp);
	void addListenerJugadorCalibrado(ListenerJugadorCalibrado *ljc);
	void addListenerManoNueva(ListenerManoNueva *lmn);
	void addListenerManoPerdida(ListenerManoPerdida *lmp);

	const Gesto * getUltimoGestoJugador(JugadorID player);
	const Gesto * getUltimoGestoReconocedor(int idRec);

};