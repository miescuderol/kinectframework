#pragma once
#include "Gesto.h"
#include "ReconocedorBasico.h"
#include "GestoPatron.h"
#include "ListenerMovimiento.h"
#include "ListenerGesto.h"
#include "Wrappers.h"

#include <vector>

//! Clase necesaria para el reconocimiento de gestos
/*!
* La clase Reconocedor provee los mecanismos necesarios para realizar el reconocimiento de UN tipo de gesto.
* Mediante objetos de tipo ReconocedorBasico, Reconocedor usa [Movimientos](@ref Movimiento) para detectar [Gestos](@ref Gesto), compar�ndolos con un GestoPatron.
* Provee interfaces para obtener el �ltimo gesto realizado (de existir), tanto con mecanismos bajo demanda o suscripci�n a eventos.
*/
class Reconocedor : public ListenerMovimiento {
public:
	Reconocedor(GestoPatron * gestoPatron, std::string idJugadorArt, ReconocedorBasico * recBasicos);
	~Reconocedor(void);

	const Gesto * getUltimoGesto();
	/*!<
	 * \return El �ltimo gesto detectado y lo elimina del buffer de salida.
	 */

	const Gesto * peekUltimoGesto();
	/*!<
	 * \return El �ltimo gesto, sin eliminarlo del buffer de salida.
	 */

	GestoPatron * getGestoPatron();
	/*!<
	 * \return gestoPatron indicado en el constructor.
	 */

	std::string getIDJugador_Art();
	/*!<
	 * \return idJugadorArt indicado en el constructor.
	 */

	void addListener(ListenerGesto * listener);
	/*!<
	 * Suscribe listener a los eventos de reconocimiento de gestos.
	 */

	void removeListener(ListenerGesto * listener);
	/*!<
	 * Elimina listener de los objetos suscriptos mediante addListener(ListenerGesto * listener).
	 */

	int getCantListeners();
	/*!<
	 * \return la cantidad de listeners suscriptos a eventos de este Reconocedor.
	 */

	void updateMovimiento(Movimiento * m);
	/*!<
	 * Realiza una iteraci�n del ciclo de actualizaci�n para la detecci�n de gestos.
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
//	JugadorID idJugador;
	std::string idJugador_Art;
	Punto3f puntoInicial;
	std::time_t tiempoInicial;
};
