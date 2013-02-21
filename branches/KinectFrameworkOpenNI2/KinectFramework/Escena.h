#pragma once

/*! \typedef int Estado 
* Utilizar int o Estado es indistinto, y fue diseñado de esta manera para agregar semántica a los encabezados de función. 
* Una práctica recomendada, sobretodo para conservar la semántica a lo largo de varias clases, es utilizar la directiva de preprocesador #define.
*/
typedef int Estado;
/*!
* La clase Escena, como su nombre lo indica, permite al usuario describir su escena y proveer los mecanismos 
* necesarios para la actualización de la misma. 
* Cabe destacar que, a diferencia de GrafoEscena, Escena es una clase puramente virtual, requiriendo que el desarrollador 
* implemente todas sus funciones, respetando los encabezados propuestos.
*/
class Escena
{
public:

	virtual void cargar(Escena * escenaAnt) = 0;
	/*!<
	* Este método es llamado cuando el objeto Escena pasa a ser la escena activa en el game loop.
	* A través de esta interfaz, se permite cargar en memoria los datos necesarios para la ejecución de la escena. 
	* En muchos videojuegos, elementos que existían en escenas pasadas, se reutilizan en escenas siguientes. 
	* Para facilitar este intercambio de estructuras internas, se provee al usuario con el objeto Escena que acaba 
	* de ser reemplazado en el game loop. Es su responsabilidad realizar un correcto casteo del objeto provisto, 
	* para que concuerde con el tipo de dato recibido. 
	* 
	* Por ejemplo, supongamos que el desarrollador creó dos objetos de clase Escena1 y Escena2 respectivamente. 
	* Ambos son hijos de la clase abstracta Escena, y representan los dos primeros niveles de su videojuego. 
	* Suponiendo que se definió un método Estructura * Escena1::devolverEstructuraInterna(), en Escena2::cargar(...), el código debería comenzar así:
	* \code{.cpp}
	* void Escena2::cargar(Escena * anterior) {
	*		Escena1 escena1 = (Escena1*) anterior;
	*		Estructura * e = escena1->devolverEstructuraInterna();
	*		// Hacer algo con esa estructura...
	* }
	* \endcode
	* \param escenaAnt escena inmediatamente anterior.
	* \sa descargar()
	*/

	virtual bool isTerminada() = 0;
	/*!<
	* Este método debe estar implementado de forma tal que devuelva true si y sólo si la escena terminó 
	* y debe ser reemplazada por la que le sigue en el GrafoEscenas, provisto al game loop.
	*/

	virtual void update() = 0;
	/*!<
	* Cada escena tiene una interfaz que será invocada una vez por iteración del ciclo de juego en el método update() de Aplicacion. 
	* Este método, debe encargarse de actualizar las estructuras internas de la escena, en base a lo obtenido a través de los [subsistemas] (\ref Subsistema).
	* 
	* Dependiendo de la complejidad y modularización del videojuego, esta función podrá solo actualizar los puntos del jugador, 
	* o realizar cosas más complejas, como actualizar la posición del personaje en la pantalla.
	*/

	virtual Estado getEstadoFinal() = 0;
	/*!<
	* Este método será llamado desde el game loop, solo cuando isTerminado() retorne true. Mediante esta interfaz, el usuario debe dar 
	* a conocer cuál es la transición que debe seguirse por el grafo para acceder a la escena siguiente.
	* 
	* Gracias a este mecanismo, el grafo puede dejar de ser completamente lineal. El ejemplo más trivial son los dos estados de 
	* terminación: GANO / PERDIO. Con estos dos estados, el grafo puede continuar hacia el siguiente nivel (GANO), o volver al menú principal (PERDIO).
	*
	*/

	virtual void descargar() = 0;
	/*!<
	* Este método permite liberar la memoria utilizada por el objeto Escena. Es importante notar cuándo se va a ejecutar esta función: después de
	* realizar la carga (cargar()) de la escena siguiente. También se debe tener en cuenta que es posible que dicha escena necesite de 
	* algunas de las estructuras del objeto a descargar. Esta responsabilidad es delegada al programador.
	*/
};

