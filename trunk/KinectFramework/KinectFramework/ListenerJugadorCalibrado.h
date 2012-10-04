#pragma once
#include "XnCppWrapper.h"

class ListenerJugadorCalibrado
{
public:
	virtual void update(XnUserID * user) = 0;
};
