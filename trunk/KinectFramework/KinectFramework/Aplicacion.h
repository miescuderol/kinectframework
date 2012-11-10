#pragma once
#include "Grafo.h"
#include "Subsistema.h"
#include "Nivel.h"
#include "Kinect.h"
#include <vector>

/// Clase básica para construir cualquier juego.
/**
* La clase aplicación maneja el game-loop. Conoce los subsistemas del juego, y los niveles.
* El orden de inicialización/actualización de los Subsistemas es configurable por el programador.
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
	 * después inicia el bucle update-draw.
	 */

	void addSubsistemaPreNivel(Subsistema * subsistema); 
	/**< 
	 * Agrega un subsistema que va a ser inicializado/actualizado
	 * antes de la inicializacion/actualización del nivel activo. 
	 */

	void addSubsistemaPostNivel(Subsistema * subsistema);
	/**< 
	 * Agrega un subsistema que va a ser inicializado/actualizado
	 * después de la inicializacion/actualización del nivel activo. 
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

