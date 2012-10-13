#pragma once
#include "Movimiento.h"

class ListenerMovimiento
{
public:
	virtual void update(Movimiento * m) = 0;
};

