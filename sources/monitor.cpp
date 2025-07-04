#include "monitor.h"

const double monitor::infinity = std::numeric_limits<double>::infinity();

void monitor::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
tiempoTotal = 0.0;
aceptados = 0.0;
tiempoPromedio = 0.0;
rechazados = 0.0;
sigma = infinity;
}
double monitor::ta(double t) {
//This function returns a double.
    return sigma;
}
void monitor::dint(double t) {

}
void monitor::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
    if (x.port == 0) { // entra el id.
        id_entra = *static_cast<double*>(x.value);
        id_auto = id_entra;
        vehiculoIngresoEnT = t;
        ingresos[id_auto] = vehiculoIngresoEnT; // id clave, vehiculoIngresoEnT valor
        printLog("Monitor: Vehiculo con ID = %f ingreso al estacionamiento en t = %f\n", id_auto, vehiculoIngresoEnT);
        
        printLog("Monitor: Lista de ingresos:\n");
        for (std::map<double, double>::iterator it = ingresos.begin(); it != ingresos.end(); ++it) {
            printLog("  ID: %f, t_ingreso: %f\n", it->first, it->second);
        }
        aceptados += 1.0;
    } else if (x.port == 1) { // sale un par, de (id,tiempo de salida)
        id_sale = *static_cast<double*>(x.value);
        id_auto = id_sale;
        vehiculoSeFueEnT = t;
        // Solo sumas si se encuentra el id en el map.
        if (ingresos.count(id_auto)) { // si es true es xq existe
            tiempoTotal += (vehiculoSeFueEnT - ingresos[id_auto]);
            printLog("Monitor: Tiempo de permanencia del vehiculo con ID = %f: %f\n", id_auto, (vehiculoSeFueEnT - ingresos[id_auto]));
            ingresos.erase(id_auto); // elimina el id del map
            printLog("Monitor: Tiempo total acumulado hasta el momento es: %f\n", tiempoTotal);
        }
    } else if (x.port == 2) { // vehiculos denegados
        printLog("Monitor: Vehiculo con ID = %f se fue del estacionamiento en t = %f\n", vehiculoSeFueEnT, t);
        rechazados += 1.0;
    }  

}
Event monitor::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
    
return Event();
}
void monitor::exit() {
//Code executed at the end of the simulation.
    // a)
    tiempoPromedio = tiempoTotal / aceptados;
    printLog("Monitor: Tiempo promedio de permanencia en el estacionamiento: %f\n", tiempoPromedio);

    // b)
    tasa_rechazo = rechazados / (aceptados + rechazados);
    printLog("Monitor: Tasa de rechazo: %f\n", tasa_rechazo);

    // c)


}