//CPP:sources/barrera_entrada.cpp
#if !defined barrera_entrada_h
#define barrera_entrada_h

#include "simulator.h"
#include "event.h"
#include "limits"
#include "queue"


class barrera_entrada: public Simulator { 
// Declare the state,
// output variables
// and parameters

bool lo;
double sigma;

static const double infinity;

public:
	barrera_entrada(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
