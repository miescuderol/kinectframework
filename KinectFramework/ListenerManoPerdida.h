#pragma once
#include "Wrappers.h"

class ListenerManoPerdida
{
public:
	virtual void updateManoPerdida(JugadorID mano) = 0;
};