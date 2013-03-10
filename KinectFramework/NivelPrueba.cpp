#include "NivelPrueba.h"
#include "MiAplicacion.h"
#include <GL/glfw.h>
#include <iostream>


NivelPrueba::NivelPrueba(Sensor * kinect, Rendering * rendering) {
	this->kinect = kinect;
	this->rendering = rendering;
}


NivelPrueba::~NivelPrueba(void) {

}

void NivelPrueba::cargar(Escena * nivelAnt) {
	ciclos = 0;
	ciclosGesto = 0;
	cantGestos = 0;
	jugadorActivo = 100000000;
	gestoDetectado = false;
	kinect->enableGenerator(Sensor::USER_GENERATOR);
	std::cout << "Cargando NivelPrueba" << std::endl;
	float i = 0;
	kinect->addListenerJugadorNuevo(this);
	kinect->addListenerJugadorPerdido(this);
	kinect->addListenerJugadorCalibrado(this);
	std::vector<JugadorID> jugadoresActivos = kinect->getJugadoresActivos();
	for (int i = 0; i < jugadoresActivos.size(); i++)
		updateJugadorCalibrado(jugadoresActivos[i]);
	while (i < 30000) i++;
	rendering->setEscena(this);
}

bool NivelPrueba::isTerminada() {
	return cantGestos > 10 || cantGestos < -5;
}

void NivelPrueba::update() {
	ciclos++;
	if (gestoDetectado)
		ciclosGesto++;
	system("cls");
	std::cout << "Nivel 1 - " << cantGestos << " puntos." << std::endl;
	std::cout << "Jugadores: ";
	for (int i = 0; i < jugadores.size(); i++)
		std::cout << jugadores[i] << " ";
	std::cout << std::endl;
	for (int i = 0; i < jugadores.size(); i++) {
		const Esqueleto * esq = kinect->getArticulaciones(jugadores[i]);
		if (esq == NULL) return;
		const Articulacion * art = esq->getArticulacion(NiTEEsqueleto::MANO_DER);
		std::cout << "Jugador " << jugadores[i] << ": " << art->getPosicion()->X() << ", "
														<< art->getPosicion()->Y() << ", "
														<< art->getPosicion()->Z() << std::endl;
	}
	if (gestoDetectado && ciclosGesto < 30) {
		std::cout << "GESTO DETECTADO" << std::endl;
		std::cout << "   Nombre: " << gesto->getNombre() << std::endl;
		std::cout << "   Desde (" << gesto->getPosInicial().X() << "," 
								 << gesto->getPosInicial().Y() << "," 
								 << gesto->getPosInicial().Z() << ")" << std::endl;
		std::cout << "   Hasta (" << gesto->getPosFinal().X() << ","
								  << gesto->getPosFinal().Y() << ","
								  << gesto->getPosFinal().Z() << ")" << std::endl;
		std::cout << "   Duracion: " << gesto->getDuracion() << "segundos." << std::endl;
	} else {
		ciclosGesto = 0;
		gestoDetectado = false;
	}
}

void NivelPrueba::updateJugadorNuevo(JugadorID user) {
	std::cout << "Jugador " << user << " encontrado en el nivel 1" << std::endl;
}

Estado NivelPrueba::getEstadoFinal() {
	if (cantGestos > 10)
		return 1;
	else 
		return 2;
}

void NivelPrueba::descargar() {
	bool rec1, rec2;
	rec1 = kinect->stopReconocedor(idRec);
	rec2 = kinect->stopReconocedor(idRec2);
	if (rec1 && rec2)
		std::cout << "Reconocedores detenidos." << std::endl;
	kinect->removeListenerJugadorCalibrado(this);
	kinect->removeListenerJugadorPerdido(this);
	kinect->removeListenerJugadorNuevo(this);
}

void NivelPrueba::updateJugadorPerdido( JugadorID user ) {
	std::cout << "updateJugadorPerdido: " << user << std::endl;
	kinect->removeListenerGesto(this, idRec);
	for (std::vector<JugadorID>::iterator it = jugadores.begin(); it != jugadores.end();) {
		if (*it == user) {
			jugadores.erase(it);
			break;
		}
	}
}

void NivelPrueba::updateJugadorCalibrado( JugadorID user ) {
	std::cout << "Jugador " << user << " calibrado." << std::endl;
	jugadorActivo = user;
	jugadores.push_back(user);

	// Gesto ARRIBA
	GestoPatron * gesto = new GestoPatron("ARRIBA");
	Movimiento arriba;
	arriba.setDireccionX(Movimiento::NO_IMPORTA);
	arriba.setDireccionY(Movimiento::ARRIBA);
	arriba.setDireccionZ(Movimiento::NO_IMPORTA);
	gesto->addMovement(arriba);
	idRec = kinect->startReconocedor(user, NiTEEsqueleto::MANO_DER, gesto);
	kinect->addListenerGesto(this, idRec);

	//Gesto ELE
	GestoPatron * ele = new GestoPatron("ELE");
	Movimiento abajo;
	abajo.setDireccionX(Movimiento::NO_IMPORTA);
	abajo.setDireccionY(Movimiento::ABAJO);
	abajo.setDireccionZ(Movimiento::NO_IMPORTA);
	ele->addMovement(abajo);
	Movimiento izquierda;
	izquierda.setDireccionX(Movimiento::IZQUIERDA);
	izquierda.setDireccionY(Movimiento::NO_IMPORTA);
	izquierda.setDireccionZ(Movimiento::NO_IMPORTA);
	ele->addMovement(izquierda);
	idRec2 = kinect->startReconocedor(user, NiTEEsqueleto::MANO_IZQ, gesto);
	kinect->addListenerGesto(this, idRec2);
}

void NivelPrueba::updateGesto( Gesto * m ) {
	std::cout << " Notificado." << std::endl;
	gestoDetectado = true;
	if (m->getNombre() == "ELE")
		cantGestos--;
	else
		cantGestos++;
	gesto = m;
}

void NivelPrueba::getColorFondo( int& r, int& g, int& b ) {
	if (jugadores.size() > 0) {
		const Esqueleto * esq = kinect->getArticulaciones(jugadores[0]);
		if (esq == NULL) return;
		const Articulacion * art = esq->getArticulacion(NiTEEsqueleto::MANO_DER);
		r = ((float)255/(float)1600)*(art->getPosicion()->X()+800);
		g = 0;
		b = 0;
	} else {
		r = 0;
		g = 0;
		b = 0;
	}
}

void NivelPrueba::renderizar()
{
	// OpenGL rendering goes here...
	glClear( GL_COLOR_BUFFER_BIT );
	int r, g, b;
	getColorFondo(r, g, b);
	std::cout << "      Rendering, RGB: " << r << "," << g << "," << b << std::endl;
	glColor3f(((float)r/255), ((float)g/255), ((float)b/255));
	glBegin(GL_QUADS); // Start drawing a quad primitive  

	glVertex3f(-1.0f, -1.0f, 0.0f); // The bottom left corner  
	glVertex3f(-1.0f, 1.0f, 0.0f); // The top left corner  
	glVertex3f(1.0f, 1.0f, 0.0f); // The top right corner  
	glVertex3f(1.0f, -1.0f, 0.0f); // The bottom right corner  

	glEnd();  
	// Swap front and back rendering buffers
	glfwSwapBuffers();
}