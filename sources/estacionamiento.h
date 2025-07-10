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

double sigma, id, r, tiempo_permanencia;
bool insertado;
std::deque<std::pair<double, double> > l;
std::pair<double, double> vehiculo;
CRandomMersenne rng;
static const double infinity;

public:
	estacionamiento(const char *n): Simulator(n), rng(5489UL) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
