#include "MiAplicacion.h"
#include "NivelPrueba.h"
#include "NivelPrueba2.h"

using namespace std;

MiAplicacion::MiAplicacion() {
	Sensor * kinect = new Kinect();
	cout << "asd" << endl;
	kinect->start();
	cout << "qwe" << endl;
	while (!kinect->isStarted());
	cout << "kinect creado" << endl;
	setSensor(kinect);
}

bool MiAplicacion::initComponentes() {

	if(!getSensor()->enableGenerator(Kinect::TipoGenerador::USER_GENERATOR)) {
		cout << "error al inicializar el generador" << endl;
		return false;
	}


	// Crear niveles
	cout << "Creando Niveles... ";
	Escena * nivelPrueba = new NivelPrueba(getSensor());
	Escena * nivelPrueba2 = new NivelPrueba2(getSensor());
	cout << "creados." << endl;

	// Crear grafo de niveles y llenarlo
	cout << "Creando Grafo de Niveles" << endl;
	GrafoEscenas * grafoNiveles = new GrafoEscenas();
	grafoNiveles->addEscena(nivelPrueba);
	cout << "Agregado nivelPrueba";
	grafoNiveles->addEscena(nivelPrueba2);
	cout << ", nivelPrueba2.";
	grafoNiveles->addArco(nivelPrueba, 1, nivelPrueba2);
	cout << "Hecho el arco entre niveles." << endl;
	grafoNiveles->setInicio(nivelPrueba);
	grafoNiveles->setFinal(nivelPrueba2);

	// Agregarle el grafo de niveles
	cout << "Agregando el grafo de niveles... ";
	setGrafoJuego(grafoNiveles);
	cout << "agregado." << endl;
	return true;
}

void MiAplicacion::draw() {
	throw std::exception("The method or operation is not implemented.");
}
