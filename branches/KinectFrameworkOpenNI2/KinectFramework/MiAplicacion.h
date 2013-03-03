#pragma once
#include "Aplicacion.h"
#include "Kinect.h"
#include "Sensor.h"
#include "Rendering.h"

class MiAplicacion :
	public Aplicacion
{
public:

	MiAplicacion();

private:

	bool initComponentes();

	void draw();

	Rendering * rendering;

};

