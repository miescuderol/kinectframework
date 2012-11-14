#pragma once
#include "XnCppWrapper.h"

class ListenerJugadorNuevo
{
public:
	virtual void updateJugadorNuevo(XnUserID user) = 0;
};