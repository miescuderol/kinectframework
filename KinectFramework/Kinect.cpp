#define DEBUG 0

#include "Kinect.h"

void Kinect::updateArticulacionesJugadores()
{
	const nite::Array<nite::UserData>& usuarios = mapaUsuarios->getUsers();
	for (int i = 0; i < usuarios.getSize(); i++)
		if (jugadores.find(usuarios[i].getId()) != jugadores.end())
			jugadores[usuarios[i].getId()] = new NiTEEsqueleto(usuarios[i].getSkeleton());
}

JugadorID Kinect::jugadorNuevo()
{
	JugadorID aux = _jugadorNuevo;
	_jugadorNuevo = -1;
	return aux;
}

JugadorID Kinect::jugadorCalibrado(Esqueleto *& esqueleto)
{
	JugadorID jugadorCalibrado = -1;
	for (int i = 0; i < jugadoresTrackeados.size(); i++)
		if (jugadores.find(jugadoresTrackeados[i]) == jugadores.end())
			jugadorCalibrado = jugadoresTrackeados[i];

	jugadoresTrackeados.clear();

	if (jugadorCalibrado != -1)
		esqueleto = new NiTEEsqueleto(mapaUsuarios->getUserById(jugadorCalibrado)->getSkeleton());
	else 
		esqueleto = NULL;

	return jugadorCalibrado;
}

JugadorID Kinect::jugadorPerdido()
{
	JugadorID aux = _jugadorPerdido;
	_jugadorPerdido = -1;
	return aux;
}

JugadorID Kinect::manoNueva( Punto3f *& mano )
{
	JugadorID aux = _manoNueva->getId();
	mano = new Punto3f(_manoNueva->getPosition().x, _manoNueva->getPosition().y, _manoNueva->getPosition().z);
	_manoNueva = NULL;
	return aux;
}

JugadorID Kinect::manoActualizada( Punto3f *& mano )
{
	JugadorID aux = _manoActualizada->getId();
	mano = new Punto3f(_manoActualizada->getPosition().x, _manoActualizada->getPosition().y, _manoActualizada->getPosition().z);
	_manoActualizada = NULL;
	return aux;
}

JugadorID Kinect::manoPerdida()
{
	JugadorID aux = _manoPerdida->getId();
	_manoPerdida = NULL;
	return aux;
}

void Kinect::setup()
{
	OpenNI::initialize();
	nite::NiTE::initialize();

	dispositivo.open(openni::ANY_DEVICE);

	mapaImagen = new VideoFrameRef();
	mapaProfundidad = new VideoFrameRef();
	mapaUsuarios = new nite::UserTrackerFrameRef();
	mapaManos = new nite::HandTrackerFrameRef();
}

void Kinect::update()
{
	// Actualiza frames de profundidad e imagen
	m_mapas.lock();
	if (camaraProfundidad.isValid())
		camaraProfundidad.readFrame(mapaProfundidad);
	if (camaraImagen.isValid())
		camaraImagen.readFrame(mapaImagen);
	if (userTracker.isValid()) {
		userTracker.readFrame(mapaUsuarios);

		// Dejamos listas las variables para los metodos jugadorNuevo(), jugadorCalibrado(), jugadorPerdido()
		const nite::Array<nite::UserData>& usuarios = mapaUsuarios->getUsers();
		for (int i = 0; i < usuarios.getSize(); ++i) {
			const nite::UserData& usuario = usuarios[i];
			if (usuario.isNew()) {
				_jugadorNuevo = usuario.getId();
				userTracker.startSkeletonTracking(_jugadorNuevo);
			} else if (usuario.isLost()) {
				_jugadorPerdido = usuario.getId();
				userTracker.stopSkeletonTracking(_jugadorPerdido);
			} else if (usuario.getSkeleton().getState() == nite::SKELETON_TRACKED) {
				jugadoresTrackeados.push_back(usuario.getId());
			}
		}

	}
	if (handTracker.isValid()) {
		handTracker.readFrame(mapaManos);

		const nite::Array<nite::HandData>& manos = mapaManos->getHands();
		for (int i = 0; i < manos.getSize(); ++i) {
			const nite::HandData& mano = manos[i];
			if (mano.isNew()) {
				_manoNueva = &mano;
			} else if (mano.isLost()) {
				_manoPerdida = &mano;
			} else if (mano.isTracking()) {
				_manoActualizada = &mano;
			}
		}
	}
	m_mapas.unlock();

	updateJugadores();

	updateReconocedoresBasicos();

	updateManos();
}

Kinect::Kinect():Sensor(){
}

bool Kinect::enableGenerator( TipoGenerador tipo )
{
	switch (tipo) {
	case Sensor::DEPTH_GENERATOR:
		if(dispositivo.isValid() && dispositivo.hasSensor(SENSOR_DEPTH)) {
			camaraProfundidad.create(dispositivo, SENSOR_DEPTH);
			if (camaraProfundidad.start() == STATUS_OK)
				return true;
			else {
				camaraProfundidad.destroy();
				return false;
			}
		} else
			return false;
		break;
	case Sensor::IMAGE_GENERATOR:
		if(dispositivo.isValid() && dispositivo.hasSensor(SENSOR_COLOR)) {
			camaraImagen.create(dispositivo, SENSOR_COLOR);
			if (camaraImagen.start() == STATUS_OK)
				return true;
			else {
				camaraImagen.destroy();
				return false;
			}
		} else
			return false;
		break;
	case Sensor::USER_GENERATOR:
		if (userTracker.create(&dispositivo) == nite::STATUS_OK)
			return true;
		else return false;
		break;
	case Sensor::HAND_GENERATOR:
		if (handTracker.create(&dispositivo) == nite::STATUS_OK)
			return true;
		else return false;
		break;
	default:
		return false;
	}
		
}

void Kinect::disableGenerator( TipoGenerador tipo )
{
	switch (tipo) {
	case Sensor::DEPTH_GENERATOR:
		camaraProfundidad.stop();
		camaraProfundidad.destroy();
		break;
	case Sensor::IMAGE_GENERATOR:
		camaraImagen.stop();
		camaraImagen.destroy();
		break;
	case Sensor::USER_GENERATOR:
		userTracker.destroy();
		break;
	case Sensor::HAND_GENERATOR:
		handTracker.destroy();
		break;
	}
}

const void * Kinect::getMapaProfundidad()
{
	m_mapas.lock();
	VideoFrameRef * aux = new VideoFrameRef();
	*aux = *mapaProfundidad;
	m_mapas.unlock();
	return aux->getData();
}

const void * Kinect::getMapaImagen()
{
	m_mapas.lock();
	VideoFrameRef * aux = new VideoFrameRef();
	*aux = *mapaImagen;
	m_mapas.unlock();
	return aux->getData();
}

const int Kinect::getXRes( TipoSensor tipo )
{
	switch (tipo) {
	case DEPTH_SENSOR:
		return camaraProfundidad.getVideoMode().getResolutionX();
		break;
	case COLOR_SENSOR:
		return camaraImagen.getVideoMode().getResolutionX();
		break;
	default:
		return -1;
	}
}

const int Kinect::getYRes( TipoSensor tipo )
{
	switch (tipo) {
	case DEPTH_SENSOR:
		return camaraProfundidad.getVideoMode().getResolutionY();
		break;
	case COLOR_SENSOR:
		return camaraImagen.getVideoMode().getResolutionY();
		break;
	default:
		return -1;
	}
}

const JugadorID * Kinect::getPixelesUsuario( JugadorID usuario )
{
	return (const JugadorID *)mapaUsuarios->getUserMap().getPixels();
}
