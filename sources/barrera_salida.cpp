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
    proc_barrera_s = false;
	sigma = infinity;
}
double barrera_salida::ta(double t) {
//This function returns a double.
    return sigma;
}
void barrera_salida::dint(double t) {
    printLog("Barrera Salida dint: INICIO | t=%f | proc_barrera_s=%d | sigma=%f\n", t, proc_barrera_s, sigma);
    proc_barrera_s = false;
    sigma = infinity;
    printLog("Barrera Salida dint: FIN | t=%f | proc_barrera_s=%d | sigma=%f\n", t, proc_barrera_s, sigma);
}
void barrera_salida::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
    printLog("Barrera Salida dext: INICIO | t=%f | proc_barrera_s=%d | sigma=%f | port=%d\n", t, proc_barrera_s, sigma, x.port);
    if (x.port == 0) {
        proc_barrera_s = true;
        r = rng.Random();
        salida_vehiculo_s = 1 + r * (3 - 1);
        id = *(double*)(x.value);
        sigma = 4 + salida_vehiculo_s + 4;
        printLog("Barrera Salida dext: se permitio la salida de vehiculo ID = %f, el auto tarda en cruzar %f . Deberia salir en: %f + 4 + %f + 4\n", id, cruce_vehiculo_s, t, salida_vehiculo_s);
    }
    printLog("Barrera Salida dext: FIN | t=%f | proc_barrera_s=%d | sigma=%f | port=%d\n", t, proc_barrera_s, sigma, x.port);
}
Event barrera_salida::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
    printLog("Barrera Salida lambda: se permitio la salida, el vehiculo con ID = %f salio en t = %f \n",id, t);
    return Event(&id, 0);
}
void barrera_salida::exit() {
//Code executed at the end of the simulation.
}
