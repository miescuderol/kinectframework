#include "NivelPrueba2.h"
#include <GL/glfw.h>
#include <iostream>

NivelPrueba2::NivelPrueba2(Sensor * kinect, Rendering * rendering) {
	this->kinect = kinect;
	this->rendering = rendering;
}


NivelPrueba2::~NivelPrueba2(void) {

}

void NivelPrueba2::cargar(Escena * nivelAnt) {
	ciclos = 0;
	ciclosGesto = 0;
	cantGestos = 0;
	jugadorActivo = 100000000;
	gestoDetectado = false;
	std::cout << "Cargando NivelPrueba2" << std::endl;
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

bool NivelPrueba2::isTerminada() {
	return cantGestos > 10;
}

void NivelPrueba2::update() {
	ciclos++;
	if (gestoDetectado)
		ciclosGesto++;
	system("cls");
	std::cout << "Nivel 2 - (" << cantGestos << "/10) gestos detectados." << std::endl;
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

void NivelPrueba2::updateJugadorNuevo(JugadorID user) {
	std::cout << "Jugador " << user << " encontrado en el nivel 2" << std::endl;
}

Estado NivelPrueba2::getEstadoFinal() {
	return 1;
}

void NivelPrueba2::descargar() {
	bool rec1, rec2;
	rec1 = kinect->stopReconocedor(idRec);
	rec2 = kinect->stopReconocedor(idRec2);
	if (rec1 && rec2)
		std::cout << "Reconocedores detenidos." << std::endl;
	kinect->removeListenerJugadorCalibrado(this);
	kinect->removeListenerJugadorPerdido(this);
	kinect->removeListenerJugadorNuevo(this);
}

void NivelPrueba2::updateJugadorPerdido( JugadorID user ) {
	std::cout << "updateJugadorPerdido: " << user << std::endl;
	kinect->removeListenerGesto(this, idRec);
	for (std::vector<JugadorID>::iterator it = jugadores.begin(); it != jugadores.end();) {
		if (*it == user) {
			jugadores.erase(it);
			break;
		}
	}
}

void NivelPrueba2::updateJugadorCalibrado( JugadorID user ) {
	std::cout << "Jugador " << user << " calibrado." << std::endl;
	jugadorActivo = user;
	jugadores.push_back(user);

	Movimiento arriba;
	arriba.setDireccionX(Movimiento::NO_IMPORTA);
	arriba.setDireccionY(Movimiento::ARRIBA);
	arriba.setDireccionZ(Movimiento::NO_IMPORTA);

	Movimiento abajo;
	abajo.setDireccionX(Movimiento::NO_IMPORTA);
	abajo.setDireccionY(Movimiento::ABAJO);
	abajo.setDireccionZ(Movimiento::NO_IMPORTA);

	Movimiento izquierda;
	izquierda.setDireccionX(Movimiento::IZQUIERDA);
	izquierda.setDireccionY(Movimiento::NO_IMPORTA);
	izquierda.setDireccionZ(Movimiento::NO_IMPORTA);

	Movimiento adelanteArriba;
	adelanteArriba.setDireccionX(Movimiento::SIN_DIRECCION);
	adelanteArriba.setDireccionY(Movimiento::ARRIBA);
	adelanteArriba.setDireccionZ(Movimiento::ADELANTE);

	// Gesto ABAJO
	GestoPatron * gesto = new GestoPatron("ABAJO");
	
	gesto->addMovement(abajo);
	idRec = kinect->startReconocedor(user, NiTEEsqueleto::MANO_IZQ, gesto);
	kinect->addListenerGesto(this, idRec);

	//Gesto AdelanteArriba - Izquierda
	GestoPatron * ele = new GestoPatron("SUPER GESTO RE DIFICIL BUENA CAPO!");
	
	ele->addMovement(adelanteArriba);
	ele->addMovement(izquierda);
	idRec2 = kinect->startReconocedor(user, NiTEEsqueleto::MANO_DER, gesto);
	kinect->addListenerGesto(this, idRec2);
}

void NivelPrueba2::updateGesto( Gesto * m ) {
	std::cout << " Notificado." << std::endl;
	gestoDetectado = true;
	cantGestos++;
	gesto = m;
}

void NivelPrueba2::getColorFondo( int& r, int& g, int& b ) {
	if (jugadores.size() > 0) {
		const Esqueleto * esq = kinect->getArticulaciones(jugadores[0]);
		if (esq == NULL) return;
		const Articulacion * art = esq->getArticulacion(NiTEEsqueleto::MANO_DER);
		g = ((float)255/(float)1600)*(art->getPosicion()->X()+800);
		r = 0;
		b = 0;
	} else {
		r = 0;
		g = 0;
		b = 0;
	}
}

void NivelPrueba2::renderizar()
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
