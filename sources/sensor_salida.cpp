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
    estado_sensor_s = false;
    sigma = infinity;
}
double sensor_salida::ta(double t) {
//This function returns a double.
    return sigma;
}
void sensor_salida::dint(double t) {
    if (!l.empty()) {
        id = l.front();
        l.pop();
        estado_sensor_s = true;
        sigma = 1.0;
    } else {
        estado_sensor_s = false;
        sigma = infinity;
    }
}
void sensor_salida::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
    /*double incomingId = *(double*)(x.value);
    printLog(">>> SS dext RECIBIDO ID=%.0f por puerto %d en t=%.6f (estado=%d, cola=%zu)\n\n",
             incomingId, x.port, t, estado_sensor_s, l.size());
    */

    if (x.port == 0) { // Evento por el puerto 0 (estacionamiento)
        if (!estado_sensor_s) { // Proceso de salida libre
            estado_sensor_s = true;
            sigma = 1.0;
            id = *(double*)(x.value);
        } else { // Proceso de salida ocupado
            aux = *(double*)(x.value);
            l.push(aux);
            if (sigma != infinity){ // para obviar un monton de casos
                sigma -= e;
            }
        }
    }
}
Event sensor_salida::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
    printLog("Sensor salida: ID: %f solicita salida en t = %f\n", id, t);
    return Event(&id, 0);
}
void sensor_salida::exit() {
//Code executed at the end of the simulation.

}
