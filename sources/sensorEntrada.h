//CPP:sources/sensorEntrada.cpp
#if !defined sensorEntrada_h
#define sensorEntrada_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"
#include <queue>
#include <limits>



class sensorEntrada: public Simulator { 
// Declare the state,
// output variables
// and parameters

	std::queue<double> q;
	bool lo;
	double sigma;

    static const double infinity;

public:
	sensorEntrada(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
