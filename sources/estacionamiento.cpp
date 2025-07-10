#include "estacionamiento.h"
#include "../random/lib/randomc.h"
#include "../random/lib/stocc.h"
#include <algorithm>

const double estacionamiento::infinity = std::numeric_limits<double>::infinity();
double tiempo_total_inesperados = 0.0;
double autos_inesperados = 0.0;

void estacionamiento::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
    va_list parameters;
    va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
    sigma = infinity;
    insertado = false;
    //tiempo_total_inesperados = 0.0;
}
double estacionamiento::ta(double t) {
//This function returns a double.
    return sigma;
}
void estacionamiento::dint(double t) {
    l.pop_front(); // saco el primer vehiculo de la cola    
    if (!l.empty()) {
        for (std::deque<std::pair<double, double> >::iterator it = l.begin(); it != l.end(); ++it) {
            it->second -= sigma;
        }
        sigma = l.front().second;
    } else {
        sigma = infinity;
    }
}
void estacionamiento::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
    if (x.port == 0) {
        if (!l.empty()) { // lo primero que se hace es actualizar los tiempos de los autos que ya habia
            printLog("Estacionamiento dext: Actualizando tiempos de los vehiculos en la cola\n");
            for (std::deque<std::pair<double, double> >::iterator it = l.begin(); it != l.end(); ++it) {
                it->second -= e;
            }
        } else {
            printLog("Estacionamiento dext: No hay vehiculos en la cola, no hay tiempos para actualizar\n");
        }
        // ahora procesamos el vehiculo que esta entrando
        id = *(double*)(x.value);
        r = rng.Random(); // numero random entre 0 y 1
        tiempo_permanencia = 120 + r * (300 - 120); // formula inversa uniforme 120 + r * (300 - 120)
        vehiculo = std::make_pair(id, tiempo_permanencia);
        printLog("Estacionamiento dext: Vehiculo %f va a estar %f dentro del estacionamiento. Sale en %f (tiempo actual + tiempo_permanencia)\n", id, tiempo_permanencia, t + tiempo_permanencia);
        if (l.empty()) {
            l.push_back(vehiculo);
            sigma = tiempo_permanencia;
        } else {
            std::deque<std::pair<double, double> >::iterator it;
            insertado = false; // lo q pasaba antes era que si el nuevo auto tenia el mayor tiempo de todos, no se insertaba
            for (it = l.begin(); it != l.end(); it++) { // insertando ordenado
                if (vehiculo.second < it->second) { // una vez que encuentra el lugar del vehiculo que entro lo inserta a la cola
                    l.insert(it, vehiculo);
                    insertado = true;
                    break;
                }
            }
            if (!insertado) {
                l.push_back(vehiculo); // si no se inserto, lo pushea al final
            }
            sigma = l.front().second; // el tiempo de salida es el del primer vehiculo de la cola
        }
        printLog("Estacionamiento dext: Vehiculos dentro del Estacionaimento:\n");
        for (std::deque<std::pair<double, double> >::iterator vit = l.begin(); vit != l.end(); ++vit) {
            printLog("  ID: %f, Tiempo: %f\n", vit->first, vit->second);
        }
    }

}
Event estacionamiento::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
    id = l.front().first; // el id del vehiculo que quiere salir es el primero de la cola
    printLog("Estacionamiento lambda: Vehiculo quiere salir en t = %f con ID = %f \n", t, id);
    return Event(&id, 0);
}
void estacionamiento::exit() {
//Code executed at the end of the simulation.
    while (!l.empty()) {
        tiempo_total_inesperados += l.front().second; // acumula el tiempo de todos los vehiculos que quedaron en la cola al terminar la simulacion
        autos_inesperados += 1.0;
        l.pop_front();
    }    
}
