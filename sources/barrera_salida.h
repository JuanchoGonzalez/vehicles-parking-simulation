//CPP:sources/barrera_salida.cpp
#if !defined barrera_salida_h
#define barrera_salida_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"
#include "limits"
#include "ctime"
#include "../random/lib/randomc.h"
#include "../random/lib/stocc.h"

class barrera_salida: public Simulator { 
// Declare the state,
// output variables
// and parameters

bool b;
double sigma;
CRandomMersenne rng;
static const double infinity;
Event salidas[2];
double finBarrera;
double vehiculoHaSalido;

public:
	barrera_salida(const char *n): Simulator(n), rng(static_cast<int>(std::time(NULL))) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
