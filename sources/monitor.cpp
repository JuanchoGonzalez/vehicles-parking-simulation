#include "monitor.h"

extern double tiempo_total_restante;
extern double vehiculos_restantes;
extern double tiempo_ocupado_global;
const double monitor::infinity = std::numeric_limits<double>::infinity();

void monitor::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
    va_list parameters;
    va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
    porcentaje_liveness = va_arg(parameters, double);
    porcentaje_liveness = porcentaje_liveness / 100.0;
    tiempoTotal = 0.0;
    aceptados = 0.0;
    tiempoPromedio = 0.0;
    rechazados = 0.0;
    sigma = infinity;
    liveness = true;
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
    if (x.port == 0) {
        id_auto = *static_cast<double*>(x.value);
        vehiculoIngresoEnT = t;
        ingresos[id_auto] = vehiculoIngresoEnT;
        printLog("Monitor: Vehiculo con ID = %f ingreso al estacionamiento en t = %f\n", id_auto, vehiculoIngresoEnT);
        aceptados += 1.0;
    } else if (x.port == 1) {
        vehiculo_salida = *(vehiculo_info*)(x.value);
        id_auto = vehiculo_salida.id;
        vehiculoSeFueEnT = t;
        if (ingresos.count(id_auto)) {
            tiempoTotal += (vehiculoSeFueEnT - ingresos[id_auto]);
            ingresos.erase(id_auto);
        }

        if (liveness) {
            for (std::deque<Salida>::iterator it = s.begin(); it != s.end(); ++it) {
                if (it->id == vehiculo_salida.id) {
                        printLog("----- Liveness Check -----\n");
                        printLog("ID: %f\n", vehiculo_salida.id);
                        printLog("Tiempo de permanencia: %f\n", it->tiempo_permanencia);
                        printLog("10%% del tiempo de permanencia: %f\n", it->tiempo_permanencia * porcentaje_liveness);
                        printLog("Tiempo en que pidió salir: %f\n", it->tiempo);
                        printLog("Tiempo real de salida: %f\n", vehiculo_salida.tiempo);
                        printLog("Diferencia entre que pidió salir y salió: %f\n", vehiculo_salida.tiempo - it->tiempo);
                    if (it->tiempo_permanencia * porcentaje_liveness < (vehiculo_salida.tiempo - it->tiempo)) {
                        liveness = false;
                        printLog("❌ Liveness VIOLADA para ID = %f\n", vehiculo_salida.id);
                    } else {
                        printLog("✅ Liveness CUMPLIDA para ID = %f\n", vehiculo_salida.id);
                    }
                    break;
                }
            }
        }
    } else if (x.port == 2) {
        printLog("Monitor: Vehiculo fue denegado, se fue del estacionamiento en t = %f\n", t);
        rechazados += 1.0;
    } else if (x.port == 3) {
        salida = *(Salida*)(x.value);
        s.push_back(salida);
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
    printLog("\n");
    printLog("Metricas:\n");
    // a)
    tiempoPromedio = (tiempoTotal + tiempo_total_restante) / (aceptados + vehiculos_restantes);
    printLog("Monitor: Tiempo promedio de permanencia en el estacionamiento: %f\n", tiempoPromedio);

    // b)
    tasa_rechazo = rechazados / (aceptados + rechazados);
    printLog("Monitor: Tasa de rechazo: %f\n", tasa_rechazo);

    // c)
    ocupacion_promedio = tiempo_ocupado_global / TIEMPO_TOTAL_SIMULACION;
    printLog("Monitor: Ocupacion promedio del estacionamiento: %f\n", ocupacion_promedio);
    printLog("\n");
}