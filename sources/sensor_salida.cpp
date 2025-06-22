#include "sensor_salida.h"

const double sensor_salida::infinity = std::numeric_limits<double>::infinity();

void sensor_salida::init(double t,...) {
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
double sensor_salida::ta(double t) {
//This function returns a double.
    return sigma;
}
void sensor_salida::dint(double t) {
    if (l.empty()) {
        b = false;
        sigma = infinity;
    } else {
        l.pop();
        b = true;
        sigma = 1.0;
    }
}
void sensor_salida::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
    if (x.port == 0 and l.empty() and !b) {
        b = true;
        sigma = 1.0;
    } else if (x.port == 0 and !l.empty() and b) {
        double* valor = static_cast<double*>(x.value);
        l.push(*valor);
    } else {
        b = false;
        sigma = 1.0;
    }
}
Event sensor_salida::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
    static double solicitarSalida = 1.0; // hace referencia a la señal
    return Event(&solicitarSalida, 0);
}
void sensor_salida::exit() {
//Code executed at the end of the simulation.

}
