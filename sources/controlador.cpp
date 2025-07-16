#include "controlador.h"
#include "../random/lib/randomc.h"
#include "../random/lib/stocc.h"

const double controlador::infinity = std::numeric_limits<double>::infinity();

void controlador::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
	va_list parameters;
	va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
	proc_entrada = false;
	proc_salida = false;
	ingreso = false;
	procesado_entrada = false;
	procesado_salida = false;
	egreso = false;
	estado_controlador = false;
	c = 0.0;
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
		// sino hay eventos pendientes controlador desocupado y esperando evento
		sigma = infinity;
		estado_controlador = false;
	} else { 
		// Buscar el primer evento procesable tanto fuera:
		// 2 opciones: 1. que venga por puerto 0 y la entrada desocupada
		// 			   2. que venga por puerto 1 y la salida desocupada
		std::deque<eventos>::iterator it = eventos_pendientes.begin();
		while (it != eventos_pendientes.end()) {
			if ((it->puerto == 0 && !proc_entrada) || (it->puerto == 1 && !proc_salida)) {
				break;
			}
			++it;
		}
		// Si se encontro un evento procesable que agarre el id y ademas lo borre de la cola
		if (it != eventos_pendientes.end()) {
			eventos e = *it;
			eventos_pendientes.erase(it);
			id = e.id;
			// pregunta por que puerto es procesable el evento y pone correspondiente proceso en ocupado
			if (e.puerto == 0) {
				proc_entrada = true;
			} else if (e.puerto == 1) {
				proc_salida = true;
			}
			r = rng.Random(); // numero random entre 0 y 1
			tiempo_respuesta = r * 3; // formula inversa uniforme 0 + r * (3 - 0) 
			sigma = tiempo_respuesta;
			// el evento actual es el id correspondiente y el puerto por donde entro
			evento_actual = eventos(id, e.puerto);
			estado_controlador = true;
		} else {
			// No hay evento procesable
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
	if (x.port == 0) { // si viene por puerto 0 es porque entra un vehiculo
		if (estado_controlador || proc_entrada) {
			// actualizo sigma si esta en proceso el controlador o si ya esta procesando la entrada y lo pongo en la cola
			sigma -= e;
			eventos_pendientes.push_back(eventos(*(double*)(x.value), x.port));
		} else {
			r = rng.Random(); // numero random entre 0 y 1
			tiempo_respuesta = r * 3; // formula inversa uniforme 0 + r * (3 - 0) 
			sigma = tiempo_respuesta;
			id = *(double*)(x.value);
			estado_controlador = true;
			proc_entrada = true;
			evento_actual = eventos(id, x.port);
		}
	}
	if (x.port == 1) { // si viene por puerto 1 es porque sale un vehiculo
		if (estado_controlador || proc_salida) {
			// actualizo sigma si esta en proceso el controlador o si ya esta procesando la salida y lo pongo en la cola
			sigma -= e;
			eventos_pendientes.push_back(eventos(*(double*)(x.value), x.port));
		} else {
			r = rng.Random();
			tiempo_respuesta = r * 3;
			sigma = tiempo_respuesta;
			id = *(double*)(x.value);
			estado_controlador = true;
			proc_salida = true;
			evento_actual = eventos(id, x.port);
		}
	}
	if (x.port == 2) { // si viene por puerto 2 es porque entro uno o se denego uno (pero proceso de entrada)
		proc_entrada = false;
		
		if (*(double*)(x.value) >= 0) { // si es mayor o igual a 0 entonces es porque vino un vehiculo que entra sino seria -1 y no sumo xq denege entrada
			if (!ocupado && c == 0) {
        		inicio_ocupacion = t;
        		ocupado = true;
				printLog("Controlador dext: El tiempo ocupado del estacionamiento es %f\n", inicio_ocupacion);
    		}
			c += 1;
		}	
		procesado_entrada = false;
		if (!estado_controlador) {
			std::deque<eventos>::iterator it = eventos_pendientes.begin();
			while (it != eventos_pendientes.end()) {
				if (it->puerto == 0 && !proc_entrada) {
					eventos e2 = *it;
					eventos_pendientes.erase(it);
					id = e2.id;
					proc_entrada = true;
					r = rng.Random(); // numero random entre 0 y 1
					tiempo_respuesta = r * 3; // formula inversa uniforme 0 + r * (3 - 0)
					sigma = tiempo_respuesta;
					estado_controlador = true;
					evento_actual = eventos(id, e2.puerto);
					procesado_entrada = true;
					break;
				}
				++it;
			}
		}
		if (!procesado_entrada) {
			sigma -= e;
		}
	}

	if (x.port == 3) { // si viene por puerto 3 es porque salio uno
		proc_salida = false;
		c -= 1;
		if (ocupado && c == 0) {
			printLog("Controlador dext: El tiempo ocupado del estacionamiento es %f\n", inicio_ocupacion);
        	tiempo_ocupado += t - inicio_ocupacion;
			printLog("Controlador dext: El tiempo ocupado del estacionamiento es %f\n", tiempo_ocupado);
        	ocupado = false;
     	}
		procesado_salida = false;
		if (!estado_controlador) {
			std::deque<eventos>::iterator it = eventos_pendientes.begin();
			while (it != eventos_pendientes.end()) {
				if (it->puerto == 1 && !proc_salida) {
					eventos e2 = *it;
					eventos_pendientes.erase(it);
					id = e2.id;
					proc_salida = true;
					r = rng.Random(); // numero random entre 0 y 1
					tiempo_respuesta = r * 3; // formula inversa uniforme 0 + r * (3 - 0)
					sigma = tiempo_respuesta;
					estado_controlador = true;
					evento_actual = eventos(id, e2.puerto);
					procesado_salida = true;
					break;
				}
				++it;
			}
		}
		if (!procesado_salida) {
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
	} else if (proc_entrada && evento_actual.puerto == 0 && c < CAPACIDAD_MAXIMA) {
		printLog("Controlador: Se permitio la entrada de ID = %f en t = %f \n", evento_actual.id, t);
		return Event(&evento_actual.id, 0);
	} else {
		printLog("Controlador: Se denego la entrada en t = %f \n", t);
		return Event(&evento_actual.id, 1);
	}
}
void controlador::exit() {
//Code executed at the end of the simulation.
}
