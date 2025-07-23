#include "controlador.h"
#include "monitor.h"
#include "../random/lib/randomc.h"
#include "../random/lib/stocc.h"

const double controlador::infinity = std::numeric_limits<double>::infinity();
double tiempo_ocupado_global = 0.0;

void controlador::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
	va_list parameters;
	va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
	c = va_arg(parameters, double);
	capacidad_maxima = va_arg(parameters, double);
	min_tiempo_respuesta = va_arg(parameters, double);
	max_tiempo_respuesta = va_arg(parameters, double);
	proc_entrada = false;
	proc_salida = false;
	entrada_encontrada = false;
	salida_encontrada = false;
	estado_controlador = false;
	sigma = infinity;
	eventos_pendientes.clear();
	ocupado = false;
	inicio_ocupacion = 0.0;
	tiempo_ocupado = 0.0;
}

double controlador::ta(double t) {
//This function returns a double.
	return sigma;
}
void controlador::dint(double t) {
	if (eventos_pendientes.empty()) {
		sigma = infinity;
		estado_controlador = false;
	} else { 
		std::deque<eventos>::iterator it = eventos_pendientes.begin();
		while (it != eventos_pendientes.end()) {
			if ((it->puerto == 0 && !proc_entrada) || (it->puerto == 1 && !proc_salida)) {
				break;
			}
			++it;
		}
		if (it != eventos_pendientes.end()) {
			eventos e = *it;
			eventos_pendientes.erase(it);
			id = e.id;
			if (e.puerto == 0) {
				proc_entrada = true;
			} else if (e.puerto == 1) {
				proc_salida = true;
			}
			r = rng.Random(); // numero random entre 0 y 1
			sigma = min_tiempo_respuesta + r * (max_tiempo_respuesta - min_tiempo_respuesta); // formula inversa uniforme 0 + r * (3 - 0) 
			evento_actual = eventos(id, e.puerto);
			estado_controlador = true;
		} else {
			sigma = infinity;
			estado_controlador = false;
		}
	}
}
void controlador::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
	if (x.port == 0) {
		if (estado_controlador || proc_entrada) {
			sigma -= e;
			eventos_pendientes.push_back(eventos(*(double*)(x.value), x.port));
		} else {
			r = rng.Random(); // numero random entre 0 y 1
			sigma = min_tiempo_respuesta + r * (max_tiempo_respuesta - min_tiempo_respuesta); // formula inversa uniforme 0 + r * (3 - 0) 
			id = *(double*)(x.value);
			estado_controlador = true;
			proc_entrada = true;
			evento_actual = eventos(id, x.port);
		}
	}
	if (x.port == 1) {
		if (estado_controlador || proc_salida) {
			sigma -= e;
			eventos_pendientes.push_back(eventos(*(double*)(x.value), x.port));
		} else {
			r = rng.Random(); // numero random entre 0 y 1
			sigma = min_tiempo_respuesta + r * (max_tiempo_respuesta - min_tiempo_respuesta); // formula inversa uniforme 0 + r * (3 - 0) 
			id = *(double*)(x.value);
			estado_controlador = true;
			proc_salida = true;
			evento_actual = eventos(id, x.port);
		}
	}
	if (x.port == 2) {
		proc_entrada = false;
		
		if (*(double*)(x.value) >= 0) {
			if (!ocupado && c == 0) {
				inicio_ocupacion = t;
				ocupado = true;
			}
			c += 1;
		}	
		entrada_encontrada = false;
		if (!estado_controlador) {
			std::deque<eventos>::iterator it = eventos_pendientes.begin();
			while (it != eventos_pendientes.end()) {
				if (it->puerto == 0 && !proc_entrada) {
					eventos e2 = *it;
					eventos_pendientes.erase(it);
					id = e2.id;
					proc_entrada = true;
					r = rng.Random(); // numero random entre 0 y 1
					sigma = min_tiempo_respuesta + r * (max_tiempo_respuesta - min_tiempo_respuesta); // formula inversa uniforme 0 + r * (3 - 0)
					estado_controlador = true;
					evento_actual = eventos(id, e2.puerto);
					entrada_encontrada = true;
					break;
				}
				++it;
			}
		}
		if (!entrada_encontrada) {
			sigma -= e;
		}
	}

	if (x.port == 3) {
		proc_salida = false;
		c -= 1;
		if (ocupado && c == 0) {
			tiempo_ocupado += t - inicio_ocupacion;
			ocupado = false;
		}
		salida_encontrada = false;
		if (!estado_controlador) {
			std::deque<eventos>::iterator it = eventos_pendientes.begin();
			while (it != eventos_pendientes.end()) {
				if (it->puerto == 1 && !proc_salida) {
					eventos e2 = *it;
					eventos_pendientes.erase(it);
					id = e2.id;
					proc_salida = true;
					r = rng.Random(); // numero random entre 0 y 1
					sigma = min_tiempo_respuesta + r * (max_tiempo_respuesta - min_tiempo_respuesta); // formula inversa uniforme 0 + r * (3 - 0)
					estado_controlador = true;
					evento_actual = eventos(id, e2.puerto);
					salida_encontrada = true;
					break;
				}
				++it;
			}
		}
		if (!salida_encontrada) {
			sigma -= e;
		}
	}
}
Event controlador::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
	if (proc_salida && evento_actual.puerto == 1) {
		printLog("Controlador: Se permitio la salida del ID = %f en t = %f \n", evento_actual.id, t);
		return Event(&evento_actual.id, 2);
	} else if (proc_entrada && evento_actual.puerto == 0 && c < capacidad_maxima) {
		printLog("Controlador: Se permitio la entrada de ID = %f en t = %f \n", evento_actual.id, t);
		return Event(&evento_actual.id, 0);
	} else {
		printLog("Controlador: Se denego la entrada en t = %f \n", t);
		return Event(&evento_actual.id, 1);
	}
}
void controlador::exit() {
	tiempo_ocupado = TIEMPO_TOTAL_SIMULACION - inicio_ocupacion;
	tiempo_ocupado_global = tiempo_ocupado;
	//Code executed at the end of the simulation.
}
