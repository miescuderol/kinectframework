#pragma once
#include "Nivel.h"
#include "Kinect.h"

class NivelPrueba2 : public Nivel
{
public:
	NivelPrueba2(Kinect * kinect);
	~NivelPrueba2();

	void cargar(Nivel * nivelAnt);

	bool isTerminado();

	void update();

	Estado getEstadoFinal();

private:

	Kinect * kinect;
	long int ciclos;

};