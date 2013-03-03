#pragma once
#include "subsistema.h"
class SubsistemaPrueba :
	public Subsistema
{
public:
	SubsistemaPrueba(int i);
	~SubsistemaPrueba(void);

	void setup();

	void update();

	void shutdown();

private:
	int i;

};

