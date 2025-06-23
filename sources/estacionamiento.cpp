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
}
double estacionamiento::ta(double t) {
//This function returns a double.
    return sigma;
}
void estacionamiento::dint(double t) {
    l.pop();
    if (l.empty()) {
        sigma = infinity;
    } else {
        sigma = l.front().second;
    }
}
struct PairSecondComparator {
    bool operator()(const std::pair<double, double>& a, const std::pair<double, double>& b) const {
        return a.second < b.second;
    }
};
void estacionamiento::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
    std::queue<std::pair<double, double> > l_prima;
    while (!l.empty()) {
        std::pair<double, double> actual = l.front();
        l.pop();
        actual.second -= e;
        l_prima.push(actual);
    }
    l = l_prima; // hasta aca ya tengo la lista con los tiempos actualizados
    double* id = static_cast<double*>(x.value);
    double tiempo = rng.IRandomX(120, 300);
    std::pair<double, double> vehiculo = std::make_pair(*id, tiempo);
    l.push(vehiculo); // acabamos de agregar el vehiculo entrante a la lista
    // Extraer todos los elementos de la cola a un vector
    std::vector<std::pair<double, double> > temp_vec;
    while (!l.empty()) {
        temp_vec.push_back(l.front());
        l.pop();
    }
    // Ordenar el vector por el segundo elemento de la tupla (de menor a mayor)
    std::sort(temp_vec.begin(), temp_vec.end(), PairSecondComparator());
    // Volver a cargar la cola con los elementos ordenados
    for (size_t i = 0; i < temp_vec.size(); ++i) {
        l.push(temp_vec[i]);
    }
}
Event estacionamiento::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
    vehiculoQuiereSalir = 1.0;
    return Event(&vehiculoQuiereSalir, 0);
}
void estacionamiento::exit() {
//Code executed at the end of the simulation.

}
