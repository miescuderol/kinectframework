#pragma once
#include "GrafoEscenas.h"
#include "Subsistema.h"
#include "Escena.h"
#include "Kinect.h"
#include "Sensor.h"
#include <vector>

//! Clase básica para construir cualquier juego.
/*!
* La clase aplicación maneja el game-loop. Conoce los subsistemas del juego, y las escenas.
* El orden de inicialización/actualización de los Subsistemas es configurable por el programador.
*/
class Aplicacion
{
public:

	Aplicacion();

	void run(); 
	/*!< 
	 * Inicia el game-loop. 
	 * Primero llama al setup de todos los subsistemas, carga la primer escena,
	 * después inicia el bucle update-draw.
	 */

protected:

	virtual void initComponentes() = 0;
	/*!<
	 * Inicializa los componentes de la Aplicacion.
	 * En este método se deben crear los objetos de subsistemas, escenas, etc.
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
	 * antes de la inicializacion/actualización de la escena activa. 
	 */

	void addSubsistemaPostEscena(Subsistema * subsistema);
	/*!< 
	 * Agrega un subsistema que va a ser inicializado/actualizado
	 * después de la inicializacion/actualización de la escena activa. 
	 */

	Sensor * getSensor();

	void setSensor(Sensor * sensor);

private:

	Sensor * sensor;
	GrafoEscenas * grafoEscenas;
	Escena * escenaActiva;
	std::vector<Subsistema*> subsistemasPreEscena, subsistemasPostEscena;

};

