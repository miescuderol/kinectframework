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

class DepthDevice
{

private:

	bool started;

	boost::thread threadKinect;

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

protected:

	void notifyAllJugadorNuevo(XnUserID jugadorNuevo);
	void notifyAllJugadorPerdido(XnUserID jugadorPerdido);
	void notifyAllJugadorCalibrado(XnUserID jugadorCalibrado);
	void notifyAllManoNueva(XnUserID manoNueva);
	void notifyAllManoPerdida(XnUserID manoPerdida);

	ReconocedorBasico * buscarReconocedorBasico(char * idRecBasico);

	void updateReconocedoresBasicos();	/**< Setea posiciones nuevas a los reconocedores basicos. */
	void updateJugadores();
	void updateManos();
	virtual void updateArticulacionesJugador(XnUserID jugador) = 0;

	virtual XnUserID manoNueva(XnPoint3D * manoNueva) = 0; /**< Devuelve el ID de la nueva mano. De no existir, -1. \param manoNueva Nueva mano encontrada */
	virtual XnUserID manoActualizada(XnPoint3D * manoActualizada) = 0; /**< Devuelve el ID de la nueva mano. De no existir, -1. \param manoNueva Nueva mano encontrada */
	virtual XnUserID manoPerdida() = 0; /**< Devuelve el ID de la nueva mano. De no existir, -1. */
	

	virtual void setup() = 0;
	virtual	void update() = 0;
	void run();


public:

	DepthDevice();

	void start();
	/**< Inicia la ejecución del componente DepthDevice en otro thread */

	void shutdown();
	/**< Termina la ejecución del componente DepthDevice */

	virtual const XnDepthPixel * getMapaProfundidad() = 0;
	virtual const XnUInt8 * getMapaImagen() = 0;
	virtual const int getXRes() = 0;
	virtual const int getYRes() = 0;
	virtual const XnPoint3D * getMano(XnUserID jugador) = 0;
	virtual const XnSkeletonJointTransformation * getArticulaciones(XnUserID jugador) = 0;
	virtual bool isTrackingPlayer(XnUserID jugador) = 0;
	virtual bool isTracking() = 0;
	virtual const XnLabel * getPixelesUsuario(XnUserID usuario) = 0;

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