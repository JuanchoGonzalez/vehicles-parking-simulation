//CPP:sources/monitor.cpp
#if !defined monitor_h
#define monitor_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"
#include <map>
#include <utility>
#include "limits"
#include "controlador.h"

class monitor: public Simulator { 
// Declare the state,
// output variables
// and parameters

double aceptados, rechazados;
double vehiculoIngresoEnT;
double vehiculoSeFueEnT;
double tiempoTotal;
double tiempoPromedio;
double id_entra;
double id_sale;
double id_auto; // ID del auto que entra o sale
double sigma;
std::map<double, double> ingresos; // id, tiempo de ingreso
double tasa_rechazo;
static const double infinity;
bool liveness;

struct Salida {
	double id;
	double tiempo_permanencia;
	double tiempo;
	double tiempo_barrera;
};

Salida salida;

std::deque<Salida> s;

struct vehiculo_info {
	double id;
	double tiempo;
};

public:
	monitor(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
