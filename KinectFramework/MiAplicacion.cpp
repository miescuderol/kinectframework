#include "MiAplicacion.h"
#include "NivelPrueba.h"
#include "NivelPrueba2.h"
#include "NivelPrueba3.h"
#include "Rendering.h"

using namespace std;

MiAplicacion::MiAplicacion() {
	Sensor * kinect = new Kinect();
	kinect->start();
	while (!kinect->isStarted());
	setSensor(kinect);
}

bool MiAplicacion::initComponentes() {

	if(!getSensor()->enableGenerator(Kinect::TipoGenerador::USER_GENERATOR)) {
		cout << "error al inicializar el generador" << endl;
		return false;
	}

	rendering = new Rendering();
	rendering->setup();

	// Crear niveles
	cout << "Creando Niveles... ";
	Escena * nivelPrueba = new NivelPrueba(getSensor(), rendering);
	Escena * nivelPrueba2 = new NivelPrueba2(getSensor(), rendering);
	Escena * nivelPrueba3 = new NivelPrueba3(getSensor(), rendering);
	cout << "creados." << endl;

	// Crear grafo de niveles y llenarlo
	cout << "Creando Grafo de Niveles" << endl;
	GrafoEscenas * grafoNiveles = new GrafoEscenas();
	grafoNiveles->addEscena(nivelPrueba);
	cout << "Agregado nivelPrueba";
	grafoNiveles->addEscena(nivelPrueba2);
	cout << ", nivelPrueba2";
	grafoNiveles->addEscena(nivelPrueba3);
	grafoNiveles->addArco(nivelPrueba, 1, nivelPrueba2);
	grafoNiveles->addArco(nivelPrueba, 2, nivelPrueba3);
	grafoNiveles->addArco(nivelPrueba2, 1, nivelPrueba);
	cout << "Hecho el arco entre niveles." << endl;
	grafoNiveles->setInicio(nivelPrueba);
	grafoNiveles->setFinal(nivelPrueba3);

	// Agregarle el grafo de niveles
	cout << "Agregando el grafo de niveles... ";
	setGrafoJuego(grafoNiveles);
	cout << "agregado." << endl;

	return true;
}

void MiAplicacion::draw() {
	if (!rendering->update())
		rendering->shutdown();
}
