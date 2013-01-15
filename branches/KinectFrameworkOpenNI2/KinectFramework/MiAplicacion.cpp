#include "MiAplicacion.h"
#include "NivelPrueba.h"
#include "NivelPrueba2.h"

using namespace std;

void MiAplicacion::initComponentes() {

	getKinect()->enableGenerator(Kinect::GeneratorType::USER_GENERATOR);

	// Crear niveles
	cout << "Creando Niveles... ";
	Nivel * nivelPrueba = new NivelPrueba(getKinect());
	Nivel * nivelPrueba2 = new NivelPrueba2(getKinect());
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

	// Agregarle el grafo de niveles
	cout << "Agregando el grafo de niveles... ";
	setGrafoJuego(grafoNiveles);
	cout << "agregado." << endl;

}
