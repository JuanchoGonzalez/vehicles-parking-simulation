//CPP:sources/generador_arribos.cpp
#if !defined generador_arribos_h
#define generador_arribos_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"
#include "ctime"
#include "../random/lib/randomc.h"
#include "../random/lib/stocc.h"

class generador_arribos: public Simulator { 
// Declare the state,
// output variables
// and parameters
double sigma;
double id;
double r;
CRandomMersenne rng;
double y;
public:
	generador_arribos(const char *n): Simulator(n), rng(5489UL) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
