//CPP:/sources/sensor_entrada.cpp
#if !defined sensor_entrada_h
#define sensor_entrada_h

#include "simulator.h"
#include "event.h"
#include "queue"
#include "limits"

class sensor_entrada: public Simulator { 
// Declare the state,
// output variables
// and parameters

std::queue<double> l;
bool estado_sensor_e;
double sigma;
double id;
double aux;

static const double infinity;

public:
	sensor_entrada(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
