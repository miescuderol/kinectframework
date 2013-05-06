#include "Kinect.h"
#include "NivelPrueba.h"
#include "NivelPrueba2.h"
#include "GrafoEscenas.h"
#include "MiAplicacion.h"
#include <iostream>
#include <fstream>
#include <boost/regex.hpp>

using namespace std;

int main(int argc, char **argv) {
	//redirecciona std::cout al archivo out.txt
	std::ofstream out("out.txt");
	//std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
	std::cout.rdbuf(out.rdbuf());

	(new MiAplicacion)->run();
}


