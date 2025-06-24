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
        l.pop();
        b = true;
        sigma = infinity;
        printLog("Sensor dint: esperando, quedan %zu en cola, b=%d\n", l.size(), b);
    } else {
        // b = false;
        sigma = infinity;
        printLog("Sensor dint: cola vacía, proceso libre, b=%d\n", b);
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
    if (!b) { // pe libre
        if (x.port == 0) { // viene por el puerto 0 (generador)
            printLog("Sensor dext: PE esta libre y llego por el puerto 0 en t = %f \n", t);
            b = true;
            sigma = 1.0;
            printLog("Sensor dext: PE ahora esta ocupado. b=%d\n", b);
        } else { // viene por el puerto 1 (barrera de entrada)
            printLog("Sensor dext: PE esta libre y llego por el puerto 1 en t = %f \n", t);
            b = false;
            sigma = infinity;
            printLog("Sensor dext: PE ahora esta esperando. b=%d\n", b);
        }
    } else { // pe ocupado
        if (x.port == 1 && l.empty()) { // viene por el puerto 1 (barrera de entrada) y no hay autos en cola
            printLog("Sensor dext: PE ocupado y llego por el puerto 1 y no hay autos en cola en t = %f \n", t);
            b = false;
            sigma -= e;
            printLog("Sensor dext: PE ahora esta libre. b=%d\n", b);
        } else if (x.port == 1 && !l.empty()) { // viene por el puerto 1 (barrera de entrada) y hay autos en cola
            printLog("Sensor dext: PE ocupado y llego por el puerto 1 y hay autos en cola en t = %f \n", t);
            sigma = 1;
            printLog("Sensor dext: PE sigue ocupado. b=%d\n", b);
        } else if (x.port == 0) {  // viene por el puerto 0 (generador)
            printLog("--------------------------------------------------------------------\n");
            printLog("Sensor dext: PE ocupado y llego por el puerto 0 en t = %f \n", t);
            double* valor = static_cast<double*>(x.value); // value es de tipo void, hay que castear sí o sí
            l.push(*valor);
            sigma -= e;
            printLog("Sensor dext: lo que llego a la cola. b=%d\n", b);
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
    detectarVehiculo = 0.0; // si es 0.0 es xq llego un vehiculo(puede o no ir a la cola)
    printLog("Sensor lambda: detectando vehiculo en t = %f \n", t);
    return Event(&detectarVehiculo, 0);
}
void sensor_entrada::exit() {
    //Code executed at the end of the simulation.
}
