#pragma once
#include "subsistema.h"
#include "MSAPhysics3D.h"
#include "Wrappers.h"
#include "Renderizable.h"
#include "Rendering.h"

using namespace msa;

class World : public Subsistema, 
			public Renderizable
{

public:
	World(int alto, int ancho, Rendering * rendering);
	~World(void);

	void setup();

	void update();

	void shutdown();

	void setArticulaciones(const Articulacion * articulaciones[3], int nroArt);

	void setParticulasVivas(int particulasVivas);

	void controlarFinNivel();

	bool terminado();

	void empezar();

	float map(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp = false);

	float random(float min, float max);

	void renderizar();

	physics::World3D * getPhysics();


private: 

	physics::World3D * physics;
	physics::Particle3D articulaciones[3];
	physics::Particle3D FIN;
	int nroArticulaciones;

	int alto, ancho, particulasVivas;
	bool empezo, termino;

	GLuint texturaParticulas, texturaFondo;
	
	Rendering * rendering;
	double old_time;

	void initParticulas();

};

