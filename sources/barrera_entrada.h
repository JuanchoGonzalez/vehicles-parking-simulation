//CPP:sources/barrera_entrada.cpp
#if !defined barrera_entrada_h
#define barrera_entrada_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"
#include "limits"
#include "ctime"
#include "../random/lib/randomc.h"
#include "../random/lib/stocc.h"

class barrera_entrada: public Simulator { 
// Declare the state,
// output variables
// and parameters

double sigma, id, r, cruce_vehiculo_e, apertura_barrera_entrada, cierre_barrera_entrada, min_cruce_e, max_cruce_e, min_salida_e, max_salida_e;
bool proc_barrera_e;
CRandomMersenne rng;
static const double infinity;

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
