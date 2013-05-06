#include "World.h"

#define ASPECTO_MUNDO 1.8
#define HORIZONTE 500

World::World(int alto, int ancho, Rendering * rendering) {
	this->physics = new physics::World3D();
	this->rendering = rendering;
	this->ancho = ancho;
	this->alto = alto;
	this->particulasVivas = 0;
}


World::~World(void)
{
}

void World::setup(){
	physics->setGravity(ofVec3f(0, 1, 0));

	// set world dimensions, not essential, but speeds up collision
	physics->setWorldSize(ofVec3f(-ancho/2, 0, -ancho/2), ofVec3f(ancho/2, alto, 0));
	physics->setDrag(0.97f);
	physics->enableCollision();
	termino = false;
	empezo = false;
	initParticulas();

	texturaParticulas = rendering->LoadTexture("data/images/powerUpCeleste.tga");
	glBindTexture(GL_TEXTURE_2D,texturaParticulas);
}

void World::update(){
	if(!empezo) return;

	physics->update();

	if (physics->numberOfParticles() < particulasVivas) {
		int vidaParticulas = 5000;
		int distRestriccion = 40000;
		float posX		= random(-ancho/2, ancho/2);
		float posY		= random(0, alto);
		float posZ		= random(-ancho/2, 0);
		float mass		= random(1, 3);
		float bounce	= 0;
		float radius	= map(mass, 1, 3, 1, 5);

		physics::Particle3D *p = physics->makeParticle(Vec3f(posX, posY, posZ));

		p->setMass(mass)->setBounce(bounce)->setRadius(4*radius)->enableCollision()->makeFree();

		physics->makeAttraction(&articulaciones[1], p, random(3, 10));
	}
	controlarFinNivel();
}

void World::shutdown()
{
	delete physics;
}

void World::setArticulaciones(const Articulacion *artis[3], int nroArt){
	this->nroArticulaciones = nroArt;
	for (int i = 0; i < nroArticulaciones; i++) {
		articulaciones[i].moveTo(Vec3f(-artis[i]->getPosicion()->X()/3, -(artis[i]->getPosicion()->Y()-alto)/3, -(artis[i]->getPosicion()->Z()-1000)/3));
	}
}

void World::setParticulasVivas(int partVivas){
	this->particulasVivas = partVivas;
}


void World::controlarFinNivel()
{
	float x1 = articulaciones[1].getPosition().x;
	float y1 = articulaciones[1].getPosition().y;
	float z1 = articulaciones[1].getPosition().z;
	float x2 = articulaciones[2].getPosition().x;
	float y2 = articulaciones[2].getPosition().y;
	float z2 = articulaciones[2].getPosition().z;

	float pux = FIN.getPosition().x;
	float puy = FIN.getPosition().y;
	float puz = FIN.getPosition().z;

	if ((pux-15 < x1 && x1 < pux+15 && puy-15 < y1 && y1 < puy+15) ||
		(pux-15 < x2 && x2 < pux+15 && puy-15 < y2 && y2 < puy+15)) {
			termino = true;
	}
}

bool World::terminado(){
	return termino;
}

void World::empezar(){
	empezo = true;
}

float World::map(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp) {

	if (fabs(inputMin - inputMax) < FLT_EPSILON){
		return outputMin;
	} else {
		float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);

		if( clamp ){
			if(outputMax < outputMin){
				if( outVal < outputMax )outVal = outputMax;
				else if( outVal > outputMin )outVal = outputMin;
			}else{
				if( outVal > outputMax )outVal = outputMax;
				else if( outVal < outputMin )outVal = outputMin;
			}
		}
		return outVal;
	}

}

float World::random(float x, float y) {
	float high = 0;
	float low = 0;
	float randNum = 0;
	// if there is no range, return the value
	if (x == y) return x; 			// float == ?, wise? epsilon?
	high = MAX(x,y);
	low = MIN(x,y);
	randNum = low + ((high-low) * rand()/(RAND_MAX + 1.0));
	return randNum;
}

physics::World3D * World::getPhysics(){
	return physics;
}

void World::initParticulas(){
	//inicializa particula FIN
	float posX		= random(-ancho/4, ancho/4);
	float posY		= random(0, alto/2);
	float posZ		= random(-ancho/4, 0);
	physics->addParticle(&FIN);
	FIN.moveTo(Vec3f(posX, posY, posZ));
	FIN.setRadius(30)->makeFixed();

	//inicializa particulas de articulaciones
	physics->addParticle(&articulaciones[0]);
	articulaciones[0].makeFixed();
	articulaciones[0].setMass(0);
	articulaciones[0].setBounce(0);
	articulaciones[0].moveTo(Vec3f(0, 256, 0));
	articulaciones[0].setRadius(15);
	physics->addParticle(&articulaciones[1]);
	articulaciones[1].makeFixed();
	articulaciones[1].setMass(1);
	articulaciones[1].setBounce(0);
	articulaciones[1].moveTo(Vec3f(30, 256, 0));
	articulaciones[1].setRadius(10);
	physics->addParticle(&articulaciones[2]);
	articulaciones[2].makeFixed();
	articulaciones[2].setMass(0);
	articulaciones[2].setBounce(0);
	articulaciones[2].moveTo(Vec3f(-30, 256, 0));
	articulaciones[2].setRadius(10);
}

void World::renderizar(){
	glLoadIdentity();
	gluLookAt(0.0, alto/2, ancho/2.0 - 80.0, 0.0, alto/2, 0.0, 0.0, -1.0, 0.0);

	glColor3f(1.0f, 1.0f, 1.0f);


	glPushMatrix();
	//	glTranslatef(anchoP/2, 0, -anchoP / 3);		// center scene
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_LIGHTING);
	glBegin(GL_QUADS);
	// draw right wall
	glColor3f(0.9, 0.9, 0.9);		glVertex3f(-ancho/2, alto, 0.0);
	glColor3f(1, 1, 1);				glVertex3f(-ancho/2, 0.0, 0.0);
	glColor3f(0.95, 0.95, 0.95);	glVertex3f(-ancho/2, 0.0, -ancho/2);
	glColor3f(0.85, 0.85, 0.85);		glVertex3f(-ancho/2, alto, -ancho/2);

	// back wall
	glColor3f(.9, 0.9, 0.9);		glVertex3f(ancho/2, alto, -ancho/2);
	glColor3f(1, 1, 1);				glVertex3f(ancho/2, 0.0, -ancho/2);
	glColor3f(0.95, 0.95, 0.95);	glVertex3f(-ancho/2, 0.0, -ancho/2);
	glColor3f(.85, 0.85, 0.85);		glVertex3f(-ancho/2, alto, -ancho/2);

	// left wall
	glColor3f(0.9, 0.9, 0.9);		glVertex3f(ancho/2, alto, -ancho/2);
	glColor3f(1, 1, 1);				glVertex3f(ancho/2, 0.0, -ancho/2);
	glColor3f(0.95, 0.95, 0.95);	glVertex3f(ancho/2, 0.0, 0.0);
	glColor3f(0.85, 0.85, 0.85);		glVertex3f(ancho/2, alto, 0.0);

	// floor
	glColor3f(0.8, 0.8, 0.8);
	glVertex3f(ancho/2, 0.0, 0.0);
	glVertex3f(ancho/2, 0.0, -ancho/2);
	glVertex3f(-ancho/2, 0.0, -ancho/2);
	glVertex3f(-ancho/2, 0.0, 0.0);

	glColor3f(0.8, 0.8, 0.8);
	glVertex3f(ancho/2, alto, 0.0);
	glVertex3f(ancho/2, alto, -ancho/2);
	glVertex3f(-ancho/2, alto, -ancho/2);
	glVertex3f(-ancho/2, alto, 0.0);
	glEnd();



	glAlphaFunc(GL_GREATER, 0.5);
	glEnable(GL_TEXTURE_2D);
	glColor4f(1, 0, 0, 1);


	for(int i=0; i< physics->numberOfParticles(); i++) {
		msa::physics::Particle3D *p = physics->getParticle(i);
		if(p->isFixed()) glColor4f(1, 0, 0, 1);
		else glColor4f(1, 1, 1, 1);

		glEnable(GL_ALPHA_TEST);

		// draw ball
		glPushMatrix();
		glTranslatef(p->getPosition().x, p->getPosition().y, p->getPosition().z);

		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(-p->getRadius(), -p->getRadius());
		glTexCoord2f(1, 0); glVertex2f(p->getRadius(), -p->getRadius());
		glTexCoord2f(1, 1); glVertex2f(p->getRadius(), p->getRadius());
		glTexCoord2f(0, 1); glVertex2f(-p->getRadius(), p->getRadius());
		glEnd();
		glPopMatrix();

		glDisable(GL_ALPHA_TEST);
		
	}
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}