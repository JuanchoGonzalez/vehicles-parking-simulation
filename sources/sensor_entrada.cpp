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
    printLog("Sensor dint: INICIO | t=%f | b=%d | sigma=%f | cola=%zu\n", t, b, sigma, l.size());
    if (!l.empty()) {
        // CASO 1 DEV
        l.pop();
        sigma = infinity;
        printLog("Sensor dint: esperando, quedan %zu en cola, b=%d\n", l.size(), b);
    } else {
        // CASO 2 DEV
        // no se pone b = false porque no se libera el proceso de entrada, aunque la cola este vacia
        // sigma = infinity xq nose cuando va a volver a llegar un vehiculo
        sigma = infinity;
        printLog("Sensor dint: cola vacía, proceso ocupado, b=%d\n", b);
        // siempre da ocupado el proceso de entrada
    }
    printLog("Sensor dint: FIN | t=%f | b=%d | sigma=%f | cola=%zu\n", t, b, sigma, l.size());
}
void sensor_entrada::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition

    
    printLog("Sensor dext: INICIO | t=%f | b=%d | sigma=%f | cola=%zu\n", t, b, sigma, l.size());

    if (x.port == 0) { // Evento por el puerto 0 (generador)
        printLog("Notificando acceso de vehiculo en t = %f\n", t);
        if (!b) { // Proceso de entrada libre
            // caso if if
            printLog("Sensor dext: PE libre y llegó por puerto 0 en t = %f\n", t);
            b = true;
            sigma = 1.0;
            printLog("Sensor dext: PE ahora está ocupado. b=%d\n", b);
        } else { // Proceso de entrada ocupado
            // CASO else else(tercero) if
            printLog("Sensor dext: PE ocupado y llegó por puerto 0 en t = %f\n", t);
            double* valor = static_cast<double*>(x.value);
            l.push(*valor);
            // if (sigma != infinity){
            printLog("SIGMA: %f\n", sigma);
            sigma -= e;
            printLog("SIGMA - e: %f\n", sigma);
            // }
            printLog("Sensor dext: Llegó a la cola. b=%d\n", b);
        }
    } else if (x.port == 1) { // Evento por el puerto 1 (barrera de entrada), implicitamente b es true
        if (l.empty()) { // 2 casos, caso if else , y caso else if
            printLog("Sensor dext: PE ocupado y llegó por puerto 1 y en t = %f\n", t);
            b = false;
            sigma = infinity;
            printLog("Sensor dext: PE ahora está esperando. b=%d\n", b);
        } else { // !l.empty()
            // podriamos hacer aca b=false y en el dint caso1 poner b=true, pero quedaria mal si viene justo un vehiculo
            // de afuera y ve el pe=libre y se manda. y estariamos dandole prioridad a el que viene de afuera que al de la cola

            // CASO else else(segundo) if
            printLog("Sensor dext: PE ocupado y llegó por puerto 1 en t = %f\n", t);
            sigma = 1.0;
            printLog("Sensor dext: PE sigue ocupado. b=%d\n", b);
        }
    }
    printLog("Sensor dext: FIN | t=%f | b=%d | sigma=%f | cola=%zu\n", t, b, sigma, l.size());
}
Event sensor_entrada::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
    detectarVehiculo = 0.0; // si es 0.0 es xq llego un vehiculo
    printLog("Sensor lambda: detectando vehiculo en t = %f \n", t);
    return Event(&detectarVehiculo, 0);
}
void sensor_entrada::exit() {
    //Code executed at the end of the simulation.
}
