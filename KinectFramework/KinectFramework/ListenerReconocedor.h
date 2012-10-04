#pragma once
#include "Reconocedor.h"

class ListenerReconocedor
{
public:
	virtual void update(Reconocedor * r) = 0;
};