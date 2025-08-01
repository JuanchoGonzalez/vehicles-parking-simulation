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
    apertura_barrera_salida = va_arg(parameters,double);
	cierre_barrera_salida = va_arg(parameters,double);
    min_cruce_s = va_arg(parameters,double);
    max_cruce_s = va_arg(parameters,double);
	sigma = infinity;
}
double barrera_salida::ta(double t) {
//This function returns a double.
    return sigma;
}
void barrera_salida::dint(double t) {
    sigma = infinity;
}
void barrera_salida::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
    if (x.port == 0) { // se permite la salida (viene el id del controlador)
        r = rng.Random();
        cruce_vehiculo_s = min_cruce_s + r * (max_cruce_s - min_cruce_s); // formula inversa uniforme 1 + r * (3 - 1)
        id = *(double*)(x.value);
        sigma = apertura_barrera_salida + cruce_vehiculo_s + cierre_barrera_salida;
        printLog("Barrera Salida dext: se permitio la salida de vehiculo ID = %f, el auto tarda en cruzar %f . Deberia salir en: %f + %f + %f + %f\n", id, cruce_vehiculo_s, t, apertura_barrera_salida, cruce_vehiculo_s, cierre_barrera_salida);
    }
}
Event barrera_salida::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
    salida.id = id;
    salida.tiempo = t;
    printLog("Barrera Salida lambda: se permitio la salida, el vehiculo con ID = %f salio en t = %f \n",id, t);
    return Event(&salida, 0);
}
void barrera_salida::exit() {
//Code executed at the end of the simulation.
}
