//CPP:sources/barrera_salida.cpp
#if !defined barrera_salida_h
#define barrera_salida_h
#define APERTURA_BARRERA_SALIDA 4
#define CIERRE_BARRERA_SALIDA 4

#include "simulator.h"
#include "event.h"
//#include "stdarg.h"
#include "limits"
#include "ctime"
#include "../random/lib/randomc.h"
#include "../random/lib/stocc.h"
#include <utility>

class barrera_salida: public Simulator { 
// Declare the state,
// output variables
// and parameters

double sigma, id, r, cruce_vehiculo_s, apertura_barrera_salida, cierre_barrera_salida, min_cruce_s, max_cruce_s;
bool proc_barrera_s;
CRandomMersenne rng;
struct Salida{
	double id;
	double tiempo;
};
Salida salida;

static const double infinity;

public:
	barrera_salida(const char *n): Simulator(n), rng(5489UL) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
