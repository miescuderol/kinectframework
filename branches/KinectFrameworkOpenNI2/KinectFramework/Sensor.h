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
#include "ListenerJugadorNuevo.h"
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
	boost::mutex m_isStarted;

	/*! Map de reconocedores asociados a un idRec */
	std::map<int, Reconocedor* > reconocedores;

	/*! Map de reconocedores básicos asociados a una articulación.*/
	std::map<std::string, ReconocedorBasico *> reconocedoresBasicos;

	/*! Vector de listeners de jugadores calibrados.*/
	std::vector<ListenerJugadorCalibrado *> listenersJugadorCalibrado;
	/*! Vector de listeners de jugadores perdidos.*/
	std::vector<ListenerJugadorPerdido *> listenersJugadorPerdido;
	/*! Vector de listeners de jugadores nuevos.*/
	std::vector<ListenerJugadorNuevo *> listenersJugadorNuevo;
	/*! Vector de listeners de manos nuevas.*/
	std::vector<ListenerManoNueva *> listenersManoNueva;
	/*! Vector de listeners de manos perdidas.*/
	std::vector<ListenerManoPerdida *> listenersManoPerdida;

protected:

	std::vector<TipoGenerador> generadoresActivos;

	/*! Map que asocia el Esqueleto a un id de jugador */
	std::map<JugadorID, Esqueleto*> jugadores;

	/*! Map que asocia una mano (Punto3f*) a un id de jugador. */
	std::map<JugadorID, Punto3f*> manos; 

	void notifyAllJugadorNuevo(JugadorID jugadorNuevo);
	/*!<
	* Notifica a todos los listeners ListenerNuevoJugador suscriptos, llamando al método UpdateJugadorNuevo de cada uno. 
	*/

	void notifyAllJugadorPerdido(JugadorID jugadorPerdido);
	/*!<
	* Notifica a todos los listeners ListenerJugadorPerdido suscriptos, llamando al método UpdateJugadorPerdido de cada uno. 
	*/

	void notifyAllJugadorCalibrado(JugadorID jugadorCalibrado);
	/*!<
	* Notifica a todos los listeners ListenerJugadorCalibrado suscriptos, llamando al método UpdateJugadorCalibrado de cada uno. 
	*/

	void notifyAllManoNueva(JugadorID manoNueva);
	/*!<
	* Notifica a todos los listeners ListenerManoNueva suscriptos, llamando al método UpdateManoNueva de cada uno. 
	*/

	void notifyAllManoPerdida(JugadorID manoPerdida);
	/*!<
	* Notifica a todos los listeners ListenerManoPerdida suscriptos, llamando al método UpdateManoPerdida de cada uno. 
	*/


	ReconocedorBasico * buscarReconocedorBasico(std::string idRecBasico);
	/*!<
	* Busca un ReconocedorBasico en el map reconocedoresBasicos en base a la clave idRecBasico y lo devuelve. 
	* En caso de no existir crea uno nuevo.
	*
	* \param idRecBasico char*, de la forma "idJugador_idArticulacion".
	* 
	* \return ReconocedorBasico (nuevo o encontrado) correspondiente al idRecBasico.
	*/

	void updateReconocedoresBasicos();	
	/*!<
	* Setea posiciones nuevas de las articulaciones correspondientes a los reconocedores basicos.
	*/

	void updateJugadores();
	/*!<
	* Verifica la existencia de cambios en los usuarios que son vistos por el sensor: usuarios nuevos, calibrados y perdidos.
	* Llama a los notificadores de cada evento encontrado.
	* 
	* Llama al método virtual updateArticulacionesJugadores.
	*/

	void updateManos();
	/*!<
	* Verifica la existencia de cambios en las manos que son vistas por el sensor: manos nuevas y perdidas.
	* Llama a los notificadores de cada evento encontrado.
	* 
	* Actualiza 
	*/

	virtual void updateArticulacionesJugadores() = 0;
	/*!<
	* Método virtual en el que se deben actualizar las posiciones de las articulaciones del Esqueleto de los jugadores registrados en el map jugadores.
	*/


	virtual JugadorID jugadorNuevo() = 0; 
	/*!< 
	* Devuelve el ID del jugador nuevo. De no existir, -1. 
	*/

	virtual JugadorID jugadorCalibrado(Esqueleto *& esqueleto) = 0; 
	/*!< 
	* Devuelve el ID del jugador calibrado. De no existir, -1. 
	* \param esqueleto El esqueleto del jugador calibrado 
	*/

	virtual JugadorID jugadorPerdido() = 0; 
	/*!< 
	* Devuelve el ID del jugador perdido. De no existir, -1. 
	*/

	virtual JugadorID manoNueva(Punto3f *& mano) = 0; 
	/*!< 
	* Devuelve el ID de la nueva mano. De no existir, -1. 
	* \param manoNueva Nueva mano encontrada 
	*/

	virtual JugadorID manoActualizada(Punto3f *& mano) = 0; 
	/*!< 
	* Devuelve el ID de la nueva mano. De no existir, -1. 
	* \param manoNueva Nueva mano encontrada 
	*/

	virtual JugadorID manoPerdida() = 0; 
	/*!< 
	* Devuelve el ID de la nueva mano. De no existir, -1. 
	*/
	

	virtual void setup() = 0;
	/*!<
	* En la implementación de este método se deben incluir las inicializaciones de las variables y estructuras necesarias.
	*/

	virtual	void update() = 0;
	/*!<
	* En este método se debe implementar las actualizaciones de las diferentes variables asociadas a los generadores activos (mapa de profundidad, mapa de imagen, etc).
	* Además se deben dejar listas las variables para los metodos jugadorNuevo(), jugadorCalibrado(), jugadorPerdido().
	*/


	virtual void stop() = 0;

	void run();
	/*!<
	* Llama a setup y luego hace un bucle while(true) en el que se llama a update hasta que se detecta una interrupción en el thread.
	*/


public:

	Sensor();

	void start();
	/*!< 
	* Inicia la ejecución del componente Sensor en otro thread. 
	*/

	void shutdown();
	/*!< 
	* Termina la ejecución del componente Sensor. 
	*/

	virtual bool enableGenerator(TipoGenerador tipo) = 0;
	/*!<
	* En la implementación este método se debe brindar los mecanismos necesarios para habilitar un generador especifico del Sensor (profundidad, imagen, etc.)
	* 
	* \param tipo Determina el [tipo de generador] (@ref TipoGenerador) que se quiere habilitar.
	*/

	virtual void disableGenerator(TipoGenerador tipo) = 0;
	/*!<
	* En la implementación este método se debe brindar los mecanismos necesarios para deshabilitar un generador especifico del Sensor (profundidad, imagen, etc.)
	* 
	* \param tipo Determina el [tipo de generador] (@ref TipoGenerador) que se quiere habilitar.
	*/

	std::vector<TipoGenerador> getActiveGenerators();
	/*!<
	* Devuelve un vector con los generadores activos.
	*
	* \return std::vector<TipoGenerador>
	*/


	virtual const void * getMapaProfundidad() = 0;
	/*!<
	* Devuelve el último frame del mapa de profundidad generado por el sensor. 
	* El valor de retorno es de tipo void* debido a que el formato del mapa de profundidad depende del SDK que se utilice.
	* 
	* \return puede ser asignado a cualquier tipo de variable puntero.
	*/

	virtual const void * getMapaImagen() = 0;
	/*!<
	* Devuelve el último frame del mapa de imagen (video RGB) generado por el sensor. 
	* El valor de retorno es de tipo void* debido a que el formato del mapa de profundidad depende del SDK que se utilice.
	* 
	* \return puede ser asignado a cualquier tipo de variable puntero.
	*/

	virtual const int getXRes( TipoSensor tipo ) = 0;
	/*!<
	* Devuelve el valor X de la resolución asociada al tipo de generador pasado como parametro.
	* 
	* \param tipo [Tipo de generador] (@ref TipoGenerador).
	*
	* \return const int
	*/

	virtual const int getYRes( TipoSensor tipo ) = 0;
	/*!<
	* Devuelve el valor Y de la resolución asociada al tipo de generador pasado como parametro.
	* 
	* \param tipo [Tipo de generador] (@ref TipoGenerador).
	*
	* \return const int
	*/

	virtual const JugadorID * getPixelesUsuario(JugadorID usuario) = 0;
	/*!<
	* Devuelve un arreglo (tamaño: y*ancho del frame) de JugadorID en donde cada posición indica si se encuentra un pixel perteneciente al 
	* usuario pasado como parámetro. Si pertenece al usuario el valor será igual al id, de lo contrario será 0.
	* 
	* \param [usuario] (@ref JugadorID).
	*
	* \return const JugadorID*. Arreglo de Y*ancho de longitud.
	*/

	const Punto3f * getMano(JugadorID jugador) const;
	/*!<
	* Devuelve la posición actual de la mano del jugador pasado como parámetro.
	* 
	* \param [jugador] (@ref JugadorID).
	*
	* \return const [Punto3f*] (@ref Punto3f). 
	*/

	const Esqueleto * getArticulaciones(JugadorID jugador) const;
	/*!<
	* Devuelve el esqueleto del jugador pasado como parametro, que contiene las posiciones de las articulaciones.
	* 
	* \param [jugador] (@ref JugadorID).
	*
	* \return const [Esqueleto*] (@ref Esqueleto). 
	*/

	bool isTrackingPlayer(JugadorID jugador);
	/*!<
	* Retorna verdadero si el jugador pasado como parámetro esta dentro del map de jugadores, es decir que está siendo trackeado por el sensor.
	* 
	* \param [jugador] (@ref JugadorID).
	*
	* \return bool.
	*/

	bool isTracking();
	/*!<
	* Retorna verdadero si hay algún jugador siendo trakeado por el sensor.
	*
	* \return bool.
	*/

	std::vector<JugadorID> getJugadoresActivos();
	/*!<
	* Devuelve un vector con todos los identificadores de jugadores activos.
	*/

	int startReconocedor(JugadorID jugador, int articulacion, GestoPatron *patron);
	/*!<
	 * Crea un nuevo reconocedor con las características pasadas por parámetro y devuelve el id del mismo.
	 * En caso de haber un reconocedor similar devuelve el id correspondiente en lugar de crear uno nuevo. 
	 * 
	 * \param [jugador] (@ref JugadorID).
	 * \param articulacion valor asociado a la articulación que se desea registrar, determinado por [TipoArticulacion] (@ref TipoArticulacion).
	 * \param [patron] (@ref GestoPatron) patrón que se desea detectar con el reconocedor.
	 *
	 * \return identificador del reconocedor.
	 */

	bool stopReconocedor(int idRec);
	/*!<
	* 
	*/


	bool isNuevoJugador(JugadorID &jugador);
	/*!<
	*
	*/

	bool isJugadorCalibrado(JugadorID &jugador);
	/*!<
	*
	*/

	bool isJugadorPerdido(JugadorID &jugador);
	/*!<
	*
	*/


	bool isNuevaMano(JugadorID &mano);
	/*!<
	*
	*/

	bool isManoPerdida(JugadorID &mano);
	/*!<
	*
	*/


	bool isStarted();
	/*!<
	*
	*/

	bool isActive( TipoGenerador tipo );
	/*!<
	*
	*/


	//métodos para agregar listeners
	void addListenerGesto(ListenerGesto *lg, int idRec);
	/*!<
	*
	*/

	void addListenerJugadorNuevo(ListenerJugadorNuevo *lnj);
	/*!<
	*
	*/

	void addListenerJugadorPerdido(ListenerJugadorPerdido *ljp);
	/*!<
	*
	*/

	void addListenerJugadorCalibrado(ListenerJugadorCalibrado *ljc);
	/*!<
	*
	*/

	void addListenerManoNueva(ListenerManoNueva *lmn);
	/*!<
	*
	*/

	void addListenerManoPerdida(ListenerManoPerdida *lmp);
	/*!<
	*
	*/

	void removeListenerGesto(ListenerGesto * lg, int idRec);
	/*!<
	*
	*/

	void removeListenerJugadorNuevo(ListenerJugadorNuevo * lnj);
	/*!<
	*
	*/

	void removeListenerJugadorPerdido(ListenerJugadorPerdido *ljp);
	/*!<
	*
	*/

	void removeListenerJugadorCalibrado(ListenerJugadorCalibrado * ljc);
	/*!<
	*
	*/

	void removeListenerManoNueva(ListenerManoNueva * lmn);
	/*!<
	*
	*/

	void removeListenerManoPerdida(ListenerManoPerdida * lmp);
	/*!<
	*
	*/

	const Gesto * getUltimoGestoJugador(JugadorID player);
	/*!<
	*
	*/

	const Gesto * getUltimoGestoReconocedor(int idRec);
	/*!<
	*
	*/

};