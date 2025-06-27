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
    id = &id_actual;
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
        double r = rng.Random();
        double tiempo_salida = 1 + r * (3 - 1);
        id_actual = *static_cast<double*>(x.value);
        sigma = 4 + tiempo_salida + 4;
        printLog("Barrera Salida: se permitio la salida de vehiculo ID = %f, el auto tarda en cruzar %d . Deberia salir en: %f + 4 + %d + 4\n", id_actual, tiempo_salida, t, tiempo_salida);
    }
}
Event barrera_salida::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
    finBarrera = 1.0;
    vehiculoHaSalido = &id_actual;
    salidas = std::make_pair(vehiculoHaSalido, finBarrera);
    printLog("Barrera Salida: se permitio la salida, el vehiculo con ID = %f salio en t = %f \n",id_actual, t);
    printLog("	Tupla de salidas que va al controlador: (ID: %f , finBarrera: %f)\n", *salidas.first, salidas.second);
    return Event(&salidas, 0);
}
void barrera_salida::exit() {
//Code executed at the end of the simulation.
}
