#pragma once
#include "XnCppWrapper.h"

class ListenerManoPerdida
{
public:
	virtual void updateManoPerdida(XnUserID mano) = 0;
};