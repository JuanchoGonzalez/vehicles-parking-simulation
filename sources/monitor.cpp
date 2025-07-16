#include "monitor.h"

extern double tiempo_total_inesperados;
extern double autos_inesperados;
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
    if (x.port == 0) { // entra un vehiculo al estacionamiento
        id_entra = *static_cast<double*>(x.value);
        id_auto = id_entra;
        vehiculoIngresoEnT = t;
        ingresos[id_auto] = vehiculoIngresoEnT; // id clave, vehiculoIngresoEnT valor
        printLog("Monitor: Vehiculo con ID = %f ingreso al estacionamiento en t = %f\n", id_auto, vehiculoIngresoEnT);
        aceptados += 1.0;
    } else if (x.port == 1) { // sale un vehiculo del estacionamiento
        vehiculo_info sal = *(vehiculo_info*)(x.value);
        id_sale = sal.id;
        id_auto = id_sale;
        vehiculoSeFueEnT = t;
        // Solo sumas si se encuentra el id en el map.
        if (ingresos.count(id_auto)) { // si es true es xq existe
            tiempoTotal += (vehiculoSeFueEnT - ingresos[id_auto]);
            ingresos.erase(id_auto); // elimina el id del map
        }

        if (liveness) {
            for (std::deque<Salida>::iterator it = s.begin(); it != s.end(); ++it) {
                if (it->id == sal.id) {
                    it->tiempo_barrera = sal.tiempo;

                        printLog("----- Liveness Check -----\n");
                        printLog("ID: %f\n", sal.id);
                        printLog("Tiempo de permanencia: %f\n", it->tiempo_permanencia);
                        printLog("1%% del tiempo de permanencia: %f\n", it->tiempo_permanencia * 0.01);
                        printLog("Tiempo en que pidió salir (lambda): %f\n", it->tiempo);
                        printLog("Tiempo real de salida (barrera): %f\n", it->tiempo_barrera);
                        printLog("Diferencia entre barrera y lambda: %f\n", it->tiempo_barrera - it->tiempo);

                    if (it->tiempo_permanencia * 0.01 < (it->tiempo_barrera - it->tiempo)) {
                        liveness = false;
                        printLog("❌ Liveness VIOLADA para ID = %f\n", sal.id);
                    } else {
                        printLog("✅ Liveness CUMPLIDA para ID = %f\n", sal.id);
                    }
                    break;
                }
            }
        }
    } else if (x.port == 2) { // vehiculos denegados
        printLog("Monitor: Vehiculo fue denegado, se fue del estacionamiento en t = %f\n", t);
        rechazados += 1.0;
    } else if (x.port == 3) {
        Salida sal = *(Salida*)(x.value);
        //sal = (5, 126.42, 240.32) (5. 253.12)
        s.push_back(sal);
    } else if (x.port == 4) {
        tiempo_ocupado = *(double*)(x.value);

        // esto hay que hacerlo me parece en c) en exit.
        ocupacion_promedio = tiempo_ocupado / t; // t es tiempo total simulado
        printLog("Monitor: Ocupación promedio del estacionamiento = %.2f%%\n", ocupacion_promedio * 100);
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
    tiempoPromedio = (tiempoTotal + tiempo_total_inesperados) / (aceptados + autos_inesperados);
    printLog("Monitor: Tiempo promedio de permanencia en el estacionamiento: %f\n", tiempoPromedio);

    // b)
    tasa_rechazo = rechazados / (aceptados + rechazados);
    printLog("Monitor: Tasa de rechazo: %f\n", tasa_rechazo);

    // c)

}