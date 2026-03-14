// includes
// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnmain.h" /Fp$(IntDir)/cmnmain.pch
#include "cmnut.h"
#include <ctime>

#include "testutils.h"


void initRandom()
{
	// seed random-number generator with current time so that numbers will be different every time we run
	std::srand(static_cast<unsigned int>(std::time(0)));
}

