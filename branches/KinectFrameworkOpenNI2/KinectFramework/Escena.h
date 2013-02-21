#pragma once

/*! \typedef int Estado 
* Utilizar int o Estado es indistinto, y fue dise�ado de esta manera para agregar sem�ntica a los encabezados de funci�n. 
* Una pr�ctica recomendada, sobretodo para conservar la sem�ntica a lo largo de varias clases, es utilizar la directiva de preprocesador #define.
*/
typedef int Estado;
/*!
* La clase Escena, como su nombre lo indica, permite al usuario describir su escena y proveer los mecanismos 
* necesarios para la actualizaci�n de la misma. 
* Cabe destacar que, a diferencia de GrafoEscena, Escena es una clase puramente virtual, requiriendo que el desarrollador 
* implemente todas sus funciones, respetando los encabezados propuestos.
*/
class Escena
{
public:

	virtual void cargar(Escena * escenaAnt) = 0;
	/*!<
	* Este m�todo es llamado cuando el objeto Escena pasa a ser la escena activa en el game loop.
	* A trav�s de esta interfaz, se permite cargar en memoria los datos necesarios para la ejecuci�n de la escena. 
	* En muchos videojuegos, elementos que exist�an en escenas pasadas, se reutilizan en escenas siguientes. 
	* Para facilitar este intercambio de estructuras internas, se provee al usuario con el objeto Escena que acaba 
	* de ser reemplazado en el game loop. Es su responsabilidad realizar un correcto casteo del objeto provisto, 
	* para que concuerde con el tipo de dato recibido. 
	* 
	* Por ejemplo, supongamos que el desarrollador cre� dos objetos de clase Escena1 y Escena2 respectivamente. 
	* Ambos son hijos de la clase abstracta Escena, y representan los dos primeros niveles de su videojuego. 
	* Suponiendo que se defini� un m�todo Estructura * Escena1::devolverEstructuraInterna(), en Escena2::cargar(...), el c�digo deber�a comenzar as�:
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
	* Este m�todo debe estar implementado de forma tal que devuelva true si y s�lo si la escena termin� 
	* y debe ser reemplazada por la que le sigue en el GrafoEscenas, provisto al game loop.
	*/

	virtual void update() = 0;
	/*!<
	* Cada escena tiene una interfaz que ser� invocada una vez por iteraci�n del ciclo de juego en el m�todo update() de Aplicacion. 
	* Este m�todo, debe encargarse de actualizar las estructuras internas de la escena, en base a lo obtenido a trav�s de los [subsistemas] (\ref Subsistema).
	* 
	* Dependiendo de la complejidad y modularizaci�n del videojuego, esta funci�n podr� solo actualizar los puntos del jugador, 
	* o realizar cosas m�s complejas, como actualizar la posici�n del personaje en la pantalla.
	*/

	virtual Estado getEstadoFinal() = 0;
	/*!<
	* Este m�todo ser� llamado desde el game loop, solo cuando isTerminado() retorne true. Mediante esta interfaz, el usuario debe dar 
	* a conocer cu�l es la transici�n que debe seguirse por el grafo para acceder a la escena siguiente.
	* 
	* Gracias a este mecanismo, el grafo puede dejar de ser completamente lineal. El ejemplo m�s trivial son los dos estados de 
	* terminaci�n: GANO / PERDIO. Con estos dos estados, el grafo puede continuar hacia el siguiente nivel (GANO), o volver al men� principal (PERDIO).
	*
	*/

	virtual void descargar() = 0;
	/*!<
	* Este m�todo permite liberar la memoria utilizada por el objeto Escena. Es importante notar cu�ndo se va a ejecutar esta funci�n: despu�s de
	* realizar la carga (cargar()) de la escena siguiente. Tambi�n se debe tener en cuenta que es posible que dicha escena necesite de 
	* algunas de las estructuras del objeto a descargar. Esta responsabilidad es delegada al programador.
	*/
};

