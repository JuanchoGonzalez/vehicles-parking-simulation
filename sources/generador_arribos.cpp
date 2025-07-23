#include "generador_arribos.h"
#include "../random/lib/randomc.cpp"
#include "../random/lib/stocc.cpp"

void generador_arribos::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
    va_list parameters;
    va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
    media = va_arg(parameters,double);
    tasa = 1/media;
    id = 0;
    r = rng.Random();
    while (r <= 0.0 || r >= 1.0) {
        r = rng.Random();
    }
    sigma = -(1.0/tasa) * log(1 - r);
}
double generador_arribos::ta(double t) {
//This function returns a double.
    return sigma;
}
void generador_arribos::dint(double t) {
    id++;
    r = rng.Random();
    while (r <= 0.0 || r >= 1.0) {
        r = rng.Random();
    }
    sigma = -(1.0/tasa) * log(1 - r);
}
void generador_arribos::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition

}
Event generador_arribos::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
    printLog("Vehiculo: %f llego en t = %f \n", id, t);
    return Event(&id, 0);
}
void generador_arribos::exit() {
//Code executed at the end of the simulation.
}
