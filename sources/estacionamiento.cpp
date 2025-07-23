#include "estacionamiento.h"
#include "../random/lib/randomc.h"
#include "../random/lib/stocc.h"
#include <algorithm>

const double estacionamiento::infinity = std::numeric_limits<double>::infinity();
double tiempo_total_restante = 0.0;
double vehiculos_restantes = 0.0;

void estacionamiento::init(double t,...) {
    va_list parameters;
    va_start(parameters,t);
    min_tiempo_permanencia = va_arg(parameters, double);
    max_tiempo_permanencia = va_arg(parameters, double);
    sigma = infinity;
    insertado = false;
    tiempo_total_restante = 0.0;
    vehiculos_restantes = 0.0;
}

double estacionamiento::ta(double t) {
    return sigma;
}

void estacionamiento::dint(double t) {
    l.pop_front(); 
    if (!l.empty()) {
        for (std::deque<vehiculo_info>::iterator it = l.begin(); it != l.end(); ++it) {
            it->permanencia -= sigma;
        }
        sigma = l.front().permanencia;
    } else {
        sigma = infinity;
    }
}

void estacionamiento::dext(Event x, double t) {
    if (x.port == 0) {
        if (!l.empty()) {
            for (std::deque<vehiculo_info>::iterator it = l.begin(); it != l.end(); ++it) {
                it->permanencia -= e;
            }
        }
        
        id = *(double*)(x.value);
        r = rng.Random();
        tiempo_permanencia = min_tiempo_permanencia + r * (max_tiempo_permanencia - min_tiempo_permanencia);
        vehiculo.id = id;
        vehiculo.permanencia = tiempo_permanencia;
        vehiculo.estacionado = tiempo_permanencia;
        printLog("Estacionamiento dext: Vehiculo %f va a estar %f dentro del estacionamiento. Sale en %f (tiempo actual + tiempo_permanencia)\n", id, tiempo_permanencia, t + tiempo_permanencia);

        if (l.empty()) {
            l.push_back(vehiculo);
            sigma = tiempo_permanencia;
        } else {
            std::deque<vehiculo_info>::iterator it;
            insertado = false;
            for (it = l.begin(); it != l.end(); it++) {
                if (vehiculo.permanencia < it->permanencia) {
                    l.insert(it, vehiculo);
                    insertado = true;
                    break;
                }
            }
            if (!insertado) {
                l.push_back(vehiculo);
            }
            sigma = l.front().permanencia;
        }

        printLog("Estacionamiento dext: Vehiculos dentro del Estacionamiento:\n");
        for (std::deque<vehiculo_info>::iterator vit = l.begin(); vit != l.end(); ++vit) {
            printLog("  ID: %f, Tiempo: %f\n", vit->id, vit->permanencia);
        }
    }
}

Event estacionamiento::lambda(double t) {
    salida.id = l.front().id;
    salida.tiempo_a_comparar = l.front().estacionado;
    salida.tiempo = t;
    printLog("Estacionamiento lambda: Vehiculo quiere salir en t = %f con ID = %f \n", t, salida.id);
    return Event(&salida, 0);
}

void estacionamiento::exit() {
    while (!l.empty()) {
        tiempo_total_restante += l.front().permanencia;
        vehiculos_restantes += 1.0;
        l.pop_front();
    }    
}
