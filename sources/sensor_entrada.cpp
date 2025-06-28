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
    estado_sensor_e = false;
    sigma = infinity;
}
double sensor_entrada::ta(double t) {
//This function returns a double.
    return sigma;
}
void sensor_entrada::dint(double t) {
    printLog("SE dint: INICIO | t=%f | estado_sensor_e=%d | sigma=%f | cola=%zu\n", t, estado_sensor_e, sigma, l.size());
    if (!l.empty()) {
        id = l.front();
        printLog("ID tope de la cola: %f\n", id);
        l.pop();
        estado_sensor_e = true;
        sigma = 1.0;
        printLog("SE dint: sensor sigue ocupado. ID: %f en proceso. estado_sensor_e=%d\n", id, estado_sensor_e);
    } else {
        printLog("SE dint: sensor libre. No hay vehiculos en la cola.\n");
        estado_sensor_e = false;
        sigma = infinity;
    }
    printLog("SE dint: FIN | t=%f | estado_sensor_e=%d | sigma=%f | cola=%zu\n", t, estado_sensor_e, sigma, l.size());
}
void sensor_entrada::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
    printLog("SE dext: INICIO | t=%f | estado_sensor_e=%d | sigma=%f | cola=%zu\n", t, estado_sensor_e, sigma, l.size());
    if (x.port == 0) { // Evento por el puerto 0 (generador)
        if (!estado_sensor_e) { // sensor libre
            printLog("SE dext: sensor libre y llegó por puerto 0 en t = %f\n", t);
            estado_sensor_e = true;
            sigma = 1.0;
            id = *(double*)(x.value);
            printLog("SE dext: sensor ocupado. ID: %f en proceso. estado_sensor_e = %d\n", id, estado_sensor_e);
        } else { // sensor ocupado
            printLog("SE dext: sensor ocupado y llegó por puerto 0 en t = %f\n", t);
            aux = *(double*)(x.value);
            l.push(aux);
            printLog("Se agrega a la cola el vehiculo ID = %f, estado_sensor_e = %d\n", aux, estado_sensor_e);
            if (sigma != infinity){ // para obviar un monton de casos
                sigma -= e;
            }
        }
    }
    printLog("SE dext: FIN | t=%f | estado_sensor_e=%d | sigma=%f | cola=%zu\n", t, estado_sensor_e, sigma, l.size());
}
Event sensor_entrada::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
    printLog("Sensor lambda: detectando vehiculo ID = %f en t = %f \n", id, t);
    return Event(&id, 0);
}
void sensor_entrada::exit() {
    //Code executed at the end of the simulation.
}
