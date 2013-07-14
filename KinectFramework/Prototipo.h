#pragma once
#include "aplicacion.h"
class Prototipo :
	public Aplicacion
{
public:
	Prototipo(void);
	~Prototipo(void);

	bool initComponentes();

	void draw();

private:

	Rendering * rendering;

};

