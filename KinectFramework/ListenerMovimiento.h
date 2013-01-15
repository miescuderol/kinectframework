#pragma once
#include "Movimiento.h"

class ListenerMovimiento
{
public:
	virtual void updateMovimiento(Movimiento * m) = 0;
};

