#pragma once
#include "XnCppWrapper.h"

class ListenerJugadorPerdido
{
public:
	virtual void updateJugadorPerdido(XnUserID user) = 0;
};