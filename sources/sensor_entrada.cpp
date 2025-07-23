#include "sensor_entrada.h"

const double sensor_entrada::infinity = std::numeric_limits<double>::infinity();

void sensor_entrada::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
    va_list parameters;
    va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
    tiempo_espera_entrada = va_arg(parameters,double);
    estado_sensor_e = false;
    sigma = infinity;
}
double sensor_entrada::ta(double t) {
//This function returns a double.
    return sigma;
}
void sensor_entrada::dint(double t) {
    if (!l.empty()) {
        id = l.front();
        l.pop();
        estado_sensor_e = true;
        sigma = tiempo_espera_entrada;
    } else {
        estado_sensor_e = false;
        sigma = infinity;
    }
}
void sensor_entrada::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
    if (x.port == 0) {
        if (!estado_sensor_e) {
            estado_sensor_e = true;
            sigma = tiempo_espera_entrada;
            id = *(double*)(x.value);
        } else {
            aux = *(double*)(x.value);
            l.push(aux);
            if (sigma != infinity){
                sigma -= e;
            }
        }
    }
}
Event sensor_entrada::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
    printLog("Sensor entrada lambda: detectando vehiculo ID = %f en t = %f \n", id, t);
    return Event(&id, 0);
}
void sensor_entrada::exit() {
    //Code executed at the end of the simulation.
}
