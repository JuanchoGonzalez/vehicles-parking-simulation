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
    printLog("SS dint: INICIO | t=%f | estado_sensor_s=%d | sigma=%f | cola=%zu\n", t, estado_sensor_s, sigma, l.size());
    if (!l.empty()) {
        id = l.front();
        printLog("ID tope de la cola: %f\n", id);
        l.pop();
        estado_sensor_s = true;
        sigma = 1.0;
        printLog("SE dint: sensor sigue ocupado. ID: %f en proceso. estado_sensor_s=%d\n", id, estado_sensor_s);
    } else {
        printLog("SE dint: sensor libre. No hay vehiculos en la cola.\n");
        estado_sensor_s = false;
        sigma = infinity;
    }
    printLog("SS dint: FIN | t=%f | estado_sensor_s=%d | sigma=%f | cola=%zu\n", t, estado_sensor_s, sigma, l.size());
}
void sensor_salida::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
    printLog("SS dext: INICIO | t=%f | estado_sensor_s=%d | sigma=%f | cola=%zu\n", t, estado_sensor_s, sigma, l.size());
    if (x.port == 0) { // Evento por el puerto 0 (estacionamiento)
        if (!estado_sensor_s) { // Proceso de salida libre
            printLog("SS dext: PS libre y llegó por puerto 0 en t = %f\n", t);
            estado_sensor_s = true;
            sigma = 1.0;
            printLog("SS dext: PS ahora está ocupado. estado_sensor_s=%d\n", estado_sensor_s);
            id = *(double*)(x.value);
        } else { // Proceso de salida ocupado
            printLog("SS dext: PS ocupado y llegó por puerto 0 en t = %f\n", t);
            aux = *(double*)(x.value);
            l.push(aux);
            printLog("Se agrega a la cola el vehiculo ID = %f, estado_sensor_s = %d\n", aux, estado_sensor_s);
            if (sigma != infinity){ // para obviar un monton de casos
                sigma -= e;
            }
        }
    }
    printLog("SS dext: FIN | t=%f | estado_sensor_s=%d | sigma=%f | cola=%zu\n", t, estado_sensor_s, sigma, l.size());
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
