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
    id = &id_actual;
}
double sensor_entrada::ta(double t) {
//This function returns a double.
    return sigma;
}
void sensor_entrada::dint(double t) {
    printLog("SE dint: INICIO | t=%f | b=%d | sigma=%f | cola=%zu\n", t, b, sigma, l.size());
    sigma = infinity;
    printLog("SE dint: FIN | t=%f | b=%d | sigma=%f | cola=%zu\n", t, b, sigma, l.size());
}
void sensor_entrada::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
    printLog("SE dext: INICIO | t=%f | b=%d | sigma=%f | cola=%zu\n", t, b, sigma, l.size());
    if (x.port == 0) { // Evento por el puerto 0 (generador)
        if (!b) { // Proceso de entrada libre
            printLog("SE dext: PE libre y llegó por puerto 0 en t = %f\n", t);
            b = true;
            sigma = 1.0;
            printLog("SE dext: PE ahora está ocupado. b=%d\n", b);
            id_actual = *static_cast<double*>(x.value);
        } else { // Proceso de entrada ocupado
            printLog("SE dext: PE ocupado y llegó por puerto 0 en t = %f\n", t);
            double* valor = static_cast<double*>(x.value);
            l.push(*valor);
            printLog("Se agrega a la cola el vehiculo ID = %f, b = %d\n", *valor, b);
            if (sigma != infinity){ // para obviar un monton de casos
                sigma -= e;
            }
        }
    } else if (x.port == 1) { // Evento por el puerto 1 (barrera de entrada), implicitamente b es true
        if (l.empty()) { // si la lista es vacia entonces esperamos
            printLog("SE dext: PE ocupado y llegó por puerto 1 y en t = %f\n", t);
            b = false;
            sigma = infinity;
            printLog("SE dext: PE ahora está esperando. b=%d\n", b);
        } else { // cuando la lista no es vacia, sacamos el del tope
            printLog("SE dext: PE ocupado y llegó por puerto 1 en t = %f\n", t);
            sigma = 1.0;
            printLog("SE dext: PE sigue ocupado. b=%d\n", b);
            id_actual = l.front(); // Tomamos el primer elemento de la cola
            l.pop(); // y lo sacamos de la cola
            printLog("SE dext: vehiculo ID = %f estaba al principio\n", *id);
        }
    }
    printLog("SE dext: FIN | t=%f | b=%d | sigma=%f | cola=%zu\n", t, b, sigma, l.size());
}
Event sensor_entrada::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
    detectarVehiculo = id;
    printLog("Sensor lambda: detectando vehiculo ID = %f en t = %f \n", *id, t);
    return Event(id, 0);
}
void sensor_entrada::exit() {
    //Code executed at the end of the simulation.
}
