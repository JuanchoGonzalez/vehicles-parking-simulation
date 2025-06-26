#include "estacionamiento.h"
#include "../random/lib/randomc.h"
#include "../random/lib/stocc.h"
#include <algorithm>

const double estacionamiento::infinity = std::numeric_limits<double>::infinity();


void estacionamiento::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
    va_list parameters;
    va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
    sigma = infinity;
    id = id_actual;
}
double estacionamiento::ta(double t) {
//This function returns a double.
    return sigma;
}
void estacionamiento::dint(double t) {
    printLog("Estacionamiento dint ");
    l.pop_front(); // saco el primer vehiculo de la cola    
    if (l.empty()) {
        sigma = infinity;
    } else {
        printLog("sigma estacionamiento");
        sigma = l.front().second - t;
    }
}
void estacionamiento::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
    id = *static_cast<double*>(x.value);
    printLog("id: %f\n", id);
    double random = rng.IRandomX(120, 300); // genero un tiempo para el vehiculo entrante
    printLog("random: %f\n", random);
    printLog("t: %f\n", t);
    double tiempo = random + t;
    std::pair<double, double> vehiculo = std::make_pair(id, tiempo);
    printLog("Vehiculo %f entro en  random + t = %f\n", id, tiempo);
    if(l.empty()) {
        l.push_back(vehiculo); // si esta sola pushea el vehiculo
        sigma = tiempo - t;
        printLog("sigma if: %f\n", sigma);
    }else {
        printLog("l.front().second() = %f\n", l.front().second);
        sigma = l.front().second - t; // el tiempo de salida es el del primer vehiculo de la cola
        printLog("sigma else: %f\n", sigma);

        std::deque<std::pair<double, double> >::iterator it;
        for (it = l.begin(); it != l.end(); it++) { // insertando ordenado
            if (vehiculo.second < it->second) { // una vez que encuentra el lugar del vehiculo que entro lo inserta a la cola
                l.insert(it, vehiculo);
                break;
            }
        }

    }
}
Event estacionamiento::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
    id_actual = l.front().first; // el id del vehiculo que quiere salir es el primero de la col
    printLog("Vehiculo quiere salir en t = %f con ID = %f \n", t, id_actual);
    vehiculoQuiereSalir = id_actual;
    return Event(&vehiculoQuiereSalir, 0);
}
void estacionamiento::exit() {
//Code executed at the end of the simulation.

}
