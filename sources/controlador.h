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

double id, r, tiempo_respuesta, sigma, c, aux, tiempo_ocupado, inicio_ocupacion, min_tiempo_respuesta, max_tiempo_respuesta, capacidad_maxima;
bool proc_entrada, proc_salida, ingreso, fin_entrada, fin_salida, estado_controlador, egreso, procesado_entrada, procesado_salida, ocupado;;
CRandomMersenne rng;
static const double infinity;
struct eventos{
	double id;
	int puerto;
	eventos() {};
	eventos(double i, int p): id(i), puerto(p) {};
};
std::deque<eventos> eventos_pendientes;
eventos evento_actual;


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
