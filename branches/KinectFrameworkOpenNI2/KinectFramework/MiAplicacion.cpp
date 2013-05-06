#include "MiAplicacion.h"
#include <GL/glfw.h>
#include "MenuPrincipal.h"
#include "NivelPrueba.h"
#include "NivelPrueba2.h"
#include "NivelPrueba3.h"
#include "Rendering.h"
#include "World.h"

using namespace std;

MiAplicacion::MiAplicacion() {
	Sensor * kinect = new Kinect();
	kinect->start();
	while (!kinect->isStarted());
	setSensor(kinect);
}

bool MiAplicacion::initComponentes() {

	if(!getSensor()->enableGenerator(Kinect::TipoGenerador::USER_GENERATOR)) {
		cout << "MiAplicacion::initComponentes error al inicializar el generador de usuarios" << endl;
		return false;
	}

	/*if(!getSensor()->enableGenerator(Kinect::TipoGenerador::HAND_GENERATOR)) {
	cout << "error al inicializar el generador de manos" << endl;
	return false;
	}*/

	rendering = new Rendering();
	rendering->setup();

	// Crear niveles
	cout << "MiAplicacion::initComponentes Creando Niveles... ";
	Escena * menuPrincipal = new MenuPrincipal(getSensor(), rendering);
	Escena * nivelPrueba = new NivelPrueba(getSensor(), rendering);
	cout << "creados." << endl;

	// Crear grafo de niveles y llenarlo
	cout << "MiAplicacion::initComponentes Creando Grafo de Niveles" << endl;
	GrafoEscenas * grafoNiveles = new GrafoEscenas();
//	grafoNiveles->addEscena(menuPrincipal);
	grafoNiveles->addEscena(nivelPrueba);
//	grafoNiveles->addArco(menuPrincipal, 1, nivelPrueba);
	cout << "Agregado nivelPrueba";
	cout << "Hecho el arco entre niveles." << endl;
	grafoNiveles->setInicio(nivelPrueba);
	grafoNiveles->setFinal(nivelPrueba);

	// Agregarle el grafo de niveles
	cout << "MiAplicacion::initComponentes Agregando el grafo de niveles... ";
	setGrafoJuego(grafoNiveles);
	cout << "agregado." << endl;

	mundo = new World(rendering->getYRes(), rendering->getXRes(), rendering);
	mundo->setup();
	addSubsistemaPostEscena(mundo);


	((NivelPrueba*)nivelPrueba)->setWorld(mundo);

	return true;
}

void MiAplicacion::draw() {
	if (!rendering->update())
		rendering->shutdown();
}
