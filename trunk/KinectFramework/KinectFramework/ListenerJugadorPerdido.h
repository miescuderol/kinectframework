#pragma once
#include "XnCppWrapper.h"

class ListenerJugadorPerdido
{
public:
	virtual void update(XnUserID * user) = 0;
};