#pragma once
#include "GrafoEscenas.h"
#include "Subsistema.h"
#include "Escena.h"
#include "Kinect.h"
#include "Sensor.h"
#include <vector>

//! Clase b�sica para construir cualquier juego.
/*!
* La clase aplicaci�n maneja el game-loop. Conoce los subsistemas del juego, y las escenas.
* El orden de inicializaci�n/actualizaci�n de los Subsistemas es configurable por el programador.
*/
class Aplicacion
{
public:

	Aplicacion();

	void run(); 
	/*!< 
	 * Inicia el game-loop. 
	 * Primero llama al setup de todos los subsistemas, carga la primer escena,
	 * despu�s inicia el bucle update-draw.
	 */

protected:

	virtual void initComponentes() = 0;
	/*!<
	 * Inicializa los componentes de la Aplicacion.
	 * En este m�todo se deben crear los objetos de subsistemas, escenas, etc.
	 */

	void setup();
	void update();
	virtual void draw() = 0;
	void exit();

	void setGrafoJuego(GrafoEscenas * grafo); 
	/*!< Agrega el grafo de niveles. */
	
	void addSubsistemaPreEscena(Subsistema * subsistema); 
	/*!< 
	 * Agrega un subsistema que va a ser inicializado/actualizado
	 * antes de la inicializacion/actualizaci�n de la escena activa. 
	 */

	void addSubsistemaPostEscena(Subsistema * subsistema);
	/*!< 
	 * Agrega un subsistema que va a ser inicializado/actualizado
	 * despu�s de la inicializacion/actualizaci�n de la escena activa. 
	 */

	Sensor * getSensor();

	void setSensor(Sensor * sensor);

private:

	Sensor * sensor;
	GrafoEscenas * grafoEscenas;
	Escena * escenaActiva;
	std::vector<Subsistema*> subsistemasPreEscena, subsistemasPostEscena;

};

