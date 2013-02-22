#pragma once
#include "Aplicacion.h"
#include "Kinect.h"
#include "Sensor.h"

class MiAplicacion :
	public Aplicacion
{
public:

	MiAplicacion();

private:

	bool initComponentes();

	void draw();

};

