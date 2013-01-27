#include "MiAplicacion.h"
#include "NivelPrueba.h"
#include "NivelPrueba2.h"

using namespace std;

MiAplicacion::MiAplicacion() {
	Sensor * kinect = new Kinect();
	kinect->start();

	while (!kinect->isStarted());
	setSensor(kinect);
}

void MiAplicacion::initComponentes() {

	getSensor()->enableGenerator(Kinect::TipoGenerador::USER_GENERATOR);

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
	grafoNiveles->addArco(nivelPrueba, Estado::SALIDA_1, nivelPrueba2);
	cout << "Hecho el arco entre niveles." << endl;
	grafoNiveles->setInicio(nivelPrueba);
	grafoNiveles->setFinal(nivelPrueba2);

	// Agregarle el grafo de niveles
	cout << "Agregando el grafo de niveles... ";
	setGrafoJuego(grafoNiveles);
	cout << "agregado." << endl;

}

void MiAplicacion::draw()
{
	throw std::exception("The method or operation is not implemented.");
}
