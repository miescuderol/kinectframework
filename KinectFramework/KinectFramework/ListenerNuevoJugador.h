#pragma once
#include "XnCppWrapper.h"

class ListenerNuevoJugador
{
public:
	virtual void updateNuevoJugador(XnUserID user) = 0;
};