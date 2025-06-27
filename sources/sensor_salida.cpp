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
    id = &id_actual;
}
double sensor_salida::ta(double t) {
//This function returns a double.
    return sigma;
}
void sensor_salida::dint(double t) {
    printLog("SS dint: INICIO | t=%f | b=%d | sigma=%f | cola=%zu\n", t, b, sigma, l.size());
    sigma = infinity;
    printLog("SS dint: FIN | t=%f | b=%d | sigma=%f | cola=%zu\n", t, b, sigma, l.size());
}
void sensor_salida::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
    printLog("SS dext: INICIO | t=%f | b=%d | sigma=%f | cola=%zu\n", t, b, sigma, l.size());
    if (x.port == 0) { // Evento por el puerto 0 (estacionamiento)
        if (!b) { // Proceso de salida libre
            printLog("SS dext: PS libre y llegó por puerto 0 en t = %f\n", t);
            b = true;
            sigma = 1.0;
            printLog("SS dext: PS ahora está ocupado. b=%d\n", b);
            id_actual = *static_cast<double*>(x.value);
            printLog("-------------------------------- ID = %f\n", id_actual);
        } else { // Proceso de salida ocupado
            printLog("SS dext: PS ocupado y llegó por puerto 0 en t = %f\n", t);
            double* valor = static_cast<double*>(x.value);
            l.push(*valor);
            printLog("Se agrega a la cola el vehiculo ID = %f, b = %d\n", *valor, b);
            if (sigma != infinity){ // para obviar un monton de casos
                sigma -= e;
            }
        }
    } else if (x.port == 1) { // Evento por el puerto 1 (barrera de salida), implicitamente b es true
        if (l.empty()) { // si la lista es vacia entonces esperamos
            printLog("SS dext: PS ocupado y llegó por puerto 1 y en t = %f\n", t);
            b = false;
            sigma = infinity;
            printLog("SS dext: PS ahora está esperando. b=%d\n", b);
        } else { // cuando la lista no es vacia, sacamos el del tope
            printLog("SS dext: PS ocupado y llegó por puerto 1 en t = %f\n", t);
            sigma = 1.0;
            printLog("SS dext: PS sigue ocupado. b=%d\n", b);
            id_actual = l.front(); // Tomamos el primer elemento de la cola
            l.pop();
        }
    }
    printLog("SS dext: FIN | t=%f | b=%d | sigma=%f | cola=%zu\n", t, b, sigma, l.size());
}
Event sensor_salida::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
    printLog("Sensor salida: Se solicita salida en t = %f\n", t);
    printLog("-------------------------------- ID = %f\n", *id);
    solicitarSalida = id;
    return Event(id, 0);
}
void sensor_salida::exit() {
//Code executed at the end of the simulation.

}
