#include "Prototipo.h"
#include "NivelPercusion1.h"


Prototipo::Prototipo(void) {
	Sensor * kinect = new Kinect();
	kinect->start();
	while (!kinect->isStarted());
	setSensor(kinect);
}


Prototipo::~Prototipo(void)
{
}

bool Prototipo::initComponentes()
{
	if(!getSensor()->enableGenerator(Kinect::TipoGenerador::USER_GENERATOR)) {
		cout << "MiAplicacion::initComponentes error al inicializar el generador de usuarios" << endl;
		return false;
	}
	rendering = new Rendering();
	rendering->setup();

	Escena * nivelPercusion = new NivelPercusion1(getSensor(), rendering);

	GrafoEscenas * grafoEscenas = new GrafoEscenas();
	grafoEscenas->addEscena(nivelPercusion);
	grafoEscenas->setInicio(nivelPercusion);
	grafoEscenas->setFinal(nivelPercusion);

	setGrafoJuego(grafoEscenas);

	return true;
}

void Prototipo::draw()
{
	if (!rendering->update())
		rendering->shutdown();
}
