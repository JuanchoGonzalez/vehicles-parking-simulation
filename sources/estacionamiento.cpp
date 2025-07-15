#include "estacionamiento.h"
#include "../random/lib/randomc.h"
#include "../random/lib/stocc.h"
#include <algorithm>

const double estacionamiento::infinity = std::numeric_limits<double>::infinity();
double tiempo_total_inesperados = 0.0;
double autos_inesperados = 0.0;

void estacionamiento::init(double t,...) {
    va_list parameters;
    va_start(parameters,t);
    sigma = infinity;
    insertado = false;
}

double estacionamiento::ta(double t) {
    return sigma;
}

void estacionamiento::dint(double t) {
    l.pop_front(); // saco el primer vehiculo de la cola    
    if (!l.empty()) {
        for (std::deque<vehiculo_info>::iterator it = l.begin(); it != l.end(); ++it) {
            it->tiempo_permanencia -= sigma;
        }
        sigma = l.front().tiempo_permanencia;
    } else {
        sigma = infinity;
    }
}

void estacionamiento::dext(Event x, double t) {
    if (x.port == 0) {
        if (!l.empty()) {
            printLog("Estacionamiento dext: Actualizando tiempos de los vehiculos en la cola\n");
            for (std::deque<vehiculo_info>::iterator it = l.begin(); it != l.end(); ++it) {
                it->tiempo_permanencia -= e;
            }
        } else {
            printLog("Estacionamiento dext: No hay vehiculos en la cola, no hay tiempos para actualizar\n");
        }

        id = *(double*)(x.value);
        r = rng.Random();
        tiempo_permanencia = 120 + r * (300 - 120);

        vehiculo_info vehiculo;
        vehiculo.id = id;
        vehiculo.tiempo_permanencia = tiempo_permanencia;
        vehiculo.tiempo_estacionado = tiempo_permanencia;

        printLog("Estacionamiento dext: Vehiculo %f va a estar %f dentro del estacionamiento. Sale en %f (tiempo actual + tiempo_permanencia)\n", id, tiempo_permanencia, t + tiempo_permanencia);

        if (l.empty()) {
            l.push_back(vehiculo);
            sigma = tiempo_permanencia;
        } else {
            std::deque<vehiculo_info>::iterator it;
            insertado = false;
            for (it = l.begin(); it != l.end(); it++) {
                if (vehiculo.tiempo_permanencia < it->tiempo_permanencia) {
                    l.insert(it, vehiculo);
                    insertado = true;
                    break;
                }
            }
            if (!insertado) {
                l.push_back(vehiculo);
            }
            sigma = l.front().tiempo_permanencia;
        }

        printLog("Estacionamiento dext: Vehiculos dentro del Estacionaimento:\n");
        for (std::deque<vehiculo_info>::iterator vit = l.begin(); vit != l.end(); ++vit) {
            printLog("  ID: %f, Tiempo: %f\n", vit->id, vit->tiempo_permanencia);
        }
    }
}

Event estacionamiento::lambda(double t) {
    salida.id = l.front().id;
    salida.tiempo_a_comparar = l.front().tiempo_estacionado;
    salida.tiempo = t;

    printLog("Estacionamiento lambda: Vehiculo quiere salir en t = %f con ID = %f \n", t, salida.id);
    printLog("Estacionamiento lambda: El vehiculo va a estar estacionado = %f\n", salida.tiempo_a_comparar);
    printLog("Estacionamiento lambda: El vehiculo salio en = %f\n", salida.tiempo);
    return Event(&salida, 0);
}

void estacionamiento::exit() {
    while (!l.empty()) {
        tiempo_total_inesperados += l.front().tiempo_permanencia;
        autos_inesperados += 1.0;
        l.pop_front();
    }    
}
