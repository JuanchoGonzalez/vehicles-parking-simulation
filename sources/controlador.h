//CPP:sources/controlador.cpp
#if !defined controlador_h
#define controlador_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"
#include "ctime"
#include <limits>
#include "../random/lib/randomc.h"
#include "../random/lib/stocc.h"



class controlador: public Simulator { 
// Declare the state,
// output variables
// and parameters

bool b,b2,ingreso;
double sigma, c , permitirSalida;
CRandomMersenne rng;
static const double infinity;
double* id;
double* vehiculoIngresando;
double* permitirEntrada;
double* denegarEntrada;
double id_actual;

public:
	controlador(const char *n): Simulator(n), rng(static_cast<int>(std::time(NULL))) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
