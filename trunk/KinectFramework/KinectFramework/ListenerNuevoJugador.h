#pragma once
#include "XnCppWrapper.h"

class ListenerNuevoJugador
{
public:
	virtual void update(XnUserID user) = 0;
};