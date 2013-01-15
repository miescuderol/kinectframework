#pragma once

#include <vector>
#include "Movimiento.h"
//#include "ListenerDirecciones.h"

class ListenerMovimiento; // forward declaration por referencia cruzada

//! Clase que convierte diferencias de posiciones en [Movimientos](@ref Movimiento).
/*!
* La clase ReconocedorBasico convierte diferencias de posiciones en [Movimientos](@ref Movimiento). 
* Permite generar un stream de posiciones absolutas en un stream de movimientos relativos unos a otros.
* Se utiliza internamente en Kinect, para emular un controlador de juegos tradicional con la entrada de datos del Kinect.
*/
class ReconocedorBasico
{

public:

	ReconocedorBasico(int divisions, float deadZone);
	/*!<
	 * Crea una instancia de ReconocedorBasico. 
	 * \param divisions determina la cantidad de divisiones en cada plano de la esfera reconocedora.
	 * \param deadZone determina la distancia en pixeles desde el centro de la esfera a partir de la cual un movimiento genera un Movimiento.
	 */
	~ReconocedorBasico(void);

	void setNewPosition(float x, float y, float z);
	/*!<
	 * Permite ingresar las nuevas posiciones a evaluar.
	 * Tiene en cuenta las anteriormente ingresadas.
	 * Si la diferencia en x, y o z es mayor que deadZone, crea un Movimiento.
	 * 
	 * \param x Posición absoluta en el eje x.
	 * \param y Posición absoluta en el eje y.
	 * \param z Posición absoluta en el eje z.
	 */

	Movimiento * lastMove();
	/*!<
	 * \return El último Movimiento generado con setNewPosition.
	 */

	void addListener(ListenerMovimiento * listener);
	/*!<
	 * Suscribe listener a los eventos de deteccion de [Movimientos](@ref Movimiento).
	 */
	
private:

	Movimiento * ultimoMovimiento;
	std::vector<ListenerMovimiento*> listeners;
	float prevX, prevY, prevZ;
	float deadZone;
	int divisions;
	float amplitude;
	float displacement;

	Movimiento::Direccion intADireccion(int i);

};

