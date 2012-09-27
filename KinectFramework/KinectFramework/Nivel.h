#pragma once

class Estado;

class Nivel
{
public:

	virtual void cargar(Nivel * nivelAnt) = 0;
	virtual bool isTerminado() = 0;
	virtual void update() = 0;
	virtual Estado * getEstadoFinal() = 0;

};

