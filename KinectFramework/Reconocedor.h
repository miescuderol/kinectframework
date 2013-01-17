#pragma once
#include "Gesto.h"
#include "ReconocedorBasico.h"
#include "GestoPatron.h"
#include "ListenerMovimiento.h"
#include "ListenerGesto.h"
#include "XnCppWrapper.h"

#include <vector>

//! Clase necesaria para el reconocimiento de gestos
/*!
* La clase Reconocedor provee los mecanismos necesarios para realizar el reconocimiento de UN tipo de gesto.
* Mediante objetos de tipo ReconocedorBasico, Reconocedor usa [Movimientos](@ref Movimiento) para detectar [Gestos](@ref Gesto), comparándolos con un GestoPatron.
* Provee interfaces para obtener el último gesto realizado (de existir), tanto con mecanismos bajo demanda o suscripción a eventos.
*/
class Reconocedor : public ListenerMovimiento {
public:
	Reconocedor(GestoPatron * gestoPatron, char* idJugadorArt, ReconocedorBasico * recBasicos);
	~Reconocedor(void);

	const Gesto * getUltimoGesto();
	/*!<
	 * \return El último gesto detectado y lo elimina del buffer de salida.
	 */

	const Gesto * peekUltimoGesto();
	/*!<
	 * \return El último gesto, sin eliminarlo del buffer de salida.
	 */

	GestoPatron * getGestoPatron();
	/*!<
	 * \return gestoPatron indicado en el constructor.
	 */

	char * getIDJugador_Art();
	/*!<
	 * \return idJugadorArt indicado en el constructor.
	 */

	void addListener(ListenerGesto * listener);
	/*!<
	 * Suscribe listener a los eventos de reconocimiento de gestos.
	 */

	void updateMovimiento(Movimiento * m);
	/*!<
	 * Realiza una iteración del ciclo de actualización para la detección de gestos.
	 */

private:

	void notifyObservers();
	
	std::vector<ListenerGesto*> listeners;

	ReconocedorBasico * recBasicos;
	Gesto * ultimoGesto;
	GestoPatron * gestoPatron;
	Movimiento * movimientoEsperado;
	int ultimoMovimiento;
//	Kinect::Joint joint;
//	XnUserID idJugador;
	char * idJugador_Art;

};

