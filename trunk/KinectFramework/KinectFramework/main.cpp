#include "Kinect.h"
#include "NivelPrueba.h"
#include "NivelPrueba2.h"
#include "Grafo.h"
#include "Aplicacion.h"
#include <iostream>

using namespace std;

int main(int argc, char **argv) {

	cout << "Creando Objeto Kinect... ";
	Kinect * kinect = new Kinect();
	cout << "creado." << endl;

	// Crear niveles
	cout << "Creando Niveles... ";
	Nivel * nivelPrueba = new NivelPrueba(kinect);
	Nivel * nivelPrueba2 = new NivelPrueba2(kinect);
	cout << "creados." << endl;

	// Crear grafo de niveles y llenarlo
	cout << "Creando Grafo de Niveles" << endl;
	Grafo * grafoNiveles = new Grafo();
	grafoNiveles->addNivel(nivelPrueba);
	cout << "Agregado nivelPrueba";
	grafoNiveles->addNivel(nivelPrueba2);
	cout << ", nivelPrueba2.";
	grafoNiveles->addArco(nivelPrueba, Estado::SALIDA_1, nivelPrueba2);
	cout << "Hecho el arco entre niveles." << endl;
	grafoNiveles->setInicio(nivelPrueba);
	grafoNiveles->setFinal(nivelPrueba2);

	// Crear aplicacion
	Aplicacion * aplicacion = new Aplicacion();
		// Agregarle el grafo de niveles
		cout << "Agregando el grafo de niveles... ";
		aplicacion->setGrafoJuego(grafoNiveles);
		cout << "agregado." << endl;
		// Agregarle un objeto Kinect
		cout << "Agregando subsistema Kinect... ";
		aplicacion->addSubsistemaPreNivel(kinect);
		cout << "agregado." << endl;


	// Llamar al run de aplicacion
	cout << "Ejecutando aplicacion..." << endl;
	aplicacion->run();

}