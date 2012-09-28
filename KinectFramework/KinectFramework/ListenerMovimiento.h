#pragma once
#include "ReconocedorBasico.h"

class ListenerMovimiento
{
public:
	virtual void update(Movimiento * m) = 0;
};

