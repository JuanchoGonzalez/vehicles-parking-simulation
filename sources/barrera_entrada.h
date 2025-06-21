//CPP:sources/barrera_entrada.cpp
#if !defined barrera_entrada_h
#define barrera_entrada_h

#include "simulator.h"
#include "event.h"
#include "limits"
#include "queue"
#include "ctime"
#include "../random/lib/randomc.h"
#include "../random/lib/stocc.h"

class barrera_entrada: public Simulator { 
// Declare the state,
// output variables
// and parameters

bool b;
double sigma;
CRandomMersenne rng;
static const double infinity;
Event salidas[2];

public:
	barrera_entrada(const char *n): Simulator(n), rng(static_cast<int>(std::time(NULL))) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
