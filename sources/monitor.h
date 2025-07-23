//CPP:sources/monitor.cpp
#if !defined monitor_h
#define monitor_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"
#include <map>
#include "limits"
#include "controlador.h"
#include "estacionamiento.h"
#define TIEMPO_TOTAL_SIMULACION 43200.0

class monitor: public Simulator { 
// Declare the state,
// output variables
// and parameters

double aceptados, rechazados, vehiculoIngresoEnT, vehiculoSeFueEnT, tiempoTotal, tiempoPromedio, porcentaje_liveness, id_auto, sigma, tiempo_ocupado, ocupacion_promedio, tasa_rechazo;
std::map<double, double> ingresos;
static const double infinity;
bool liveness;

struct Salida {
	double id;
	double tiempo_permanencia;
	double tiempo;
};
Salida salida;
std::deque<Salida> s;

struct vehiculo_info {
	double id;
	double tiempo;
};
vehiculo_info vehiculo_salida;

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
