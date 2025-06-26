//CPP:sources/estacionamiento.cpp
#if !defined estacionamiento_h
#define estacionamiento_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"
#include "limits"
#include "queue"
#include "ctime"
#include "../random/lib/randomc.h"
#include "../random/lib/stocc.h"

class estacionamiento: public Simulator { 
// Declare the state,
// output variables
// and parameters

std::deque<std::pair<double, double> > l;
double sigma;
CRandomMersenne rng;
static const double infinity;
double vehiculoQuiereSalir;
double id, id_actual;

public:
	estacionamiento(const char *n): Simulator(n), rng(static_cast<int>(std::time(NULL))) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
