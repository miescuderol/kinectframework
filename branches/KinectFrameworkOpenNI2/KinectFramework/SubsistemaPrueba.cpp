#include "SubsistemaPrueba.h"
#include <iostream>

SubsistemaPrueba::SubsistemaPrueba(int i) {
	this->i = i;
}


SubsistemaPrueba::~SubsistemaPrueba(void) {
}

void SubsistemaPrueba::setup() {
	
}

void SubsistemaPrueba::update() {
	std::cout << "Haciendo update de subsistema " << i << std::endl;
}

void SubsistemaPrueba::shutdown() {
	
}
