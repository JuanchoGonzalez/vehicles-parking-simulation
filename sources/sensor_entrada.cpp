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
    b = false;
    sigma = infinity;
}
double sensor_entrada::ta(double t) {
//This function returns a double.
    return sigma;
}
void sensor_entrada::dint(double t) {
    if (!l.empty()) {
        l.pop();
        b = true;
        sigma = 1.0;
        printLog("Sensor: desencolando auto, quedan %zu en cola, t=%f\n", l.size(), t);
    } else {
        b = false;
        sigma = infinity;
        printLog("Sensor: cola vacía, proceso libre, t=%f\n", t);
    }
}
void sensor_entrada::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
    if (!b) { // pe libre
        if (x.port == 0) {
            b = true;
            sigma = 1.0;
        } else {
            b = false;
            sigma = infinity;
        }
    } else if (x.port == 1) { // pe ocupado
        b = true;
        sigma -= e;
    } else {
        double* valor = static_cast<double*>(x.value); // value es de tipo void, hay que castear sí o sí
        l.push(*valor);
        sigma -= e;
    }
}
Event sensor_entrada::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
    detectarVehiculo = 0.0; // si es 0.0 es xq llego un vehiculo(puede o no ir a la cola)
    return Event(&detectarVehiculo, 0);
}
void sensor_entrada::exit() {
    //Code executed at the end of the simulation.
    printf("Finalizando sensor_entrada...\n");
}
