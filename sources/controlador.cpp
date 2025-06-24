#include "controlador.h"
#include "../random/lib/randomc.h"
#include "../random/lib/stocc.h"

const double estacionamiento::infinity = std::numeric_limits<double>::infinity();

void controlador::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type

b=false;
b2=false;
c=0;
sigma=infinity;

}
double controlador::ta(double t) {
//This function returns a double.
return sigma;
}
void controlador::dint(double t) {
if (x.port == 0 || x.port == 1){
	sigma = infinity;
}
}
void controlador::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition

if (x.port == 0 && c < 30.0 and !b) {
	b = true;
	sigma = rng.IRandomX(0,3);
	while (sigma = 0) {
		sigma = rng.IRandomX(0,3);
	}
}else if(x.port == 0 && c >= 30.0) {
	b = false;
	sigma = rng.IRandomX(0,3);
	while (sigma = 0) {
		sigma = rng.IRandomX(0,3);
	}
}else if(x.port == 1 && !b2) {
	b2 = true;
	sigma = rng.IRandomX(0,3);
	while (sigma = 0) {
		sigma = rng.IRandomX(0,3);
	}
}else if(x.port == 2) {
	b = false;
	c += 1.0;
	sigma = infinity;
}else if(x.port == 3) {
	b2 = false;
	c -= 1.0;
	sigma = infinity;
}
}
Event controlador::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)


return Event();
}
void controlador::exit() {
//Code executed at the end of the simulation.
printf("salida del controlador");
}
