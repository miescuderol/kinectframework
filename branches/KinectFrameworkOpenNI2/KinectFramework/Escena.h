#pragma once

enum Estado {SALIDA_1, SALIDA_2, MUERTO};

class Escena
{
public:

	virtual void cargar(Escena * escenaAnt) = 0;
	virtual bool isTerminado() = 0;
	virtual void update() = 0;
	virtual Estado getEstadoFinal() = 0;

};

