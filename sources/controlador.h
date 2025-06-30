//CPP:sources/controlador.cpp
#if !defined controlador_h
#define controlador_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"
#include "ctime"
#include "queue"
#include <limits>
#include "../random/lib/randomc.h"
#include "../random/lib/stocc.h"



class controlador: public Simulator { 
// Declare the state,
// output variables
// and parameters

double id_entrada, id_salida, id_cola_entrada, id_cola_salida, r, tiempo_respuesta, puerto_cola;
double sigma, c;
bool proc_entrada, proc_salida, ingreso, fin_entrada, fin_salida;
CRandomMersenne rng;
static const double infinity;
std::deque<std::pair<double, double> > l_entrada;
std::deque<std::pair<double, double> > l_salida;
std::pair<double, double> vehiculo;

public:
	controlador(const char *n): Simulator(n), rng(5489UL) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
