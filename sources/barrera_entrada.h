//CPP:sources/barrera_entrada.cpp
#if !defined barrera_entrada_h
#define barrera_entrada_h
#define APERTURA_BARRERA_ENTRADA 4
#define CIERRE_BARRERA_ENTRADA 4

#include "simulator.h"
#include "event.h"
#include "limits"
#include "ctime"
#include "../random/lib/randomc.h"
#include "../random/lib/stocc.h"
#include <utility>

class barrera_entrada: public Simulator { 
// Declare the state,
// output variables
// and parameters

double sigma, id, r, cruce_vehiculo_e, salida_vehiculo_e;
bool proc_barrera_e;
CRandomMersenne rng;
static const double infinity;

public:
	barrera_entrada(const char *n): Simulator(n), rng(5489UL) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
