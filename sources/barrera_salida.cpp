#include "barrera_salida.h"
#include "../random/lib/randomc.h"
#include "../random/lib/stocc.h"

const double barrera_salida::infinity = std::numeric_limits<double>::infinity();

void barrera_salida::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
    va_list parameters;
    va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
    b = false;
	sigma = infinity;
}
double barrera_salida::ta(double t) {
//This function returns a double.
    return sigma;
}
void barrera_salida::dint(double t) {
    b = false;
    sigma = infinity;
}
void barrera_salida::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
    if (x.port == 0) {
        b = true;
        sigma = 4 + rng.IRandomX(1, 3) + 4;
    }
}
Event barrera_salida::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
    finBarrera = 1.0;
	vehiculoHaSalido = 1.0;
	salidas[0] = (Event(&vehiculoHaSalido, 0)); // puerto 0 para vehiculoHaSalido
	salidas[1] = (Event(&finBarrera, 1)); // puerto 1 para fin de barrera
	return Event(&salidas, 0);
}
void barrera_salida::exit() {
//Code executed at the end of the simulation.

}
