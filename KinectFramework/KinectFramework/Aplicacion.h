#pragma once
#include "Grafo.h"
#include "Subsistema.h"
#include "Nivel.h"
#include "Kinect.h"
#include <vector>

/// Clase b�sica para construir cualquier juego.
/**
* La clase aplicaci�n maneja el game-loop. Conoce los subsistemas del juego, y los niveles.
* El orden de inicializaci�n/actualizaci�n de los Subsistemas es configurable por el programador.
*/
class Aplicacion
{
public:
	Aplicacion(void);
	~Aplicacion(void);

	void setGrafoJuego(Grafo * grafo); /**< Agrega el grafo de niveles. */

	void run(); 
	/**< 
	 * Inicia el game-loop. 
	 * Primero llama al setup de todos los subsistemas, carga el primer nivel,
	 * despu�s inicia el bucle update-draw.
	 */

	void addSubsistemaPreNivel(Subsistema * subsistema); 
	/**< 
	 * Agrega un subsistema que va a ser inicializado/actualizado
	 * antes de la inicializacion/actualizaci�n del nivel activo. 
	 */

	void addSubsistemaPostNivel(Subsistema * subsistema);
	/**< 
	 * Agrega un subsistema que va a ser inicializado/actualizado
	 * despu�s de la inicializacion/actualizaci�n del nivel activo. 
	 */

private:

	void setup();
	void update();
	void draw();
	void exit();

	Grafo * grafoNiveles;
	Nivel * nivelActivo;
	std::vector<Subsistema*> subsistemasPreNivel, subsistemasPostNivel;
};

