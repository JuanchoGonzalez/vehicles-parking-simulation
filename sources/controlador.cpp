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
	egreso = false;
	estado_controlador = false;
	c = 0;
	sigma = infinity;
	eventos_pendientes.clear();
}

double controlador::ta(double t) {
//This function returns a double.
	return sigma;
}
void controlador::dint(double t) {
	printLog("Controlador dint: INICIO | t=%f | sigma=%f | ec=%s | pe=%s | ps=%s | eventos pendientes=%zu\n", t, sigma, estado_controlador ? "true" : "false", proc_entrada ? "true" : "false", proc_salida ? "true" : "false", eventos_pendientes.size());
	if (eventos_pendientes.empty()) {
		sigma = infinity;
		estado_controlador = false;
	} else {
		// Buscar el primer evento procesable
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
				printLog("Controlador dint: Procesando evento de entrada: ID = %f, PUERTO = %d\n", id, e.puerto);
			} else if (e.puerto == 1) {
				proc_salida = true;
				printLog("Controlador dint: Procesando evento de salida: ID = %f, PUERTO = %d\n", id, e.puerto);
			}
			r = rng.Random();
			tiempo_respuesta = r * 3;
			sigma = tiempo_respuesta;
			evento_actual = eventos(id, e.puerto);
			printLog("tiempo de respuesta que sigue: %f\n", sigma);
			estado_controlador = true;
		} else {
			// No hay evento procesable
			sigma = infinity;
			estado_controlador = false;
		}
	}
	printLog("Controlador dint: FIN | t=%f | sigma=%f | ec=%s | pe=%s | ps=%s | eventos pendientes=%zu\n", t, sigma, estado_controlador ? "true" : "false", proc_entrada ? "true" : "false", proc_salida ? "true" : "false", eventos_pendientes.size());
}
void controlador::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
	printLog("Controlador dext: INICIO | t=%f | sigma=%f | ec=%s | pe=%s | ps=%s | eventos pendientes=%zu\n", t, sigma, estado_controlador ? "true" : "false", proc_entrada ? "true" : "false", proc_salida ? "true" : "false", eventos_pendientes.size());
	if (x.port == 0) {
		if (estado_controlador || proc_entrada) {
			printLog("Controlador dext: Se agrego evento pendiente: ID = %f, PUERTO = %d\n", *(double*)(x.value), x.port);
			sigma -= e;
			eventos_pendientes.push_back(eventos(*(double*)(x.value), x.port));
		} else if (!proc_entrada) {
			r = rng.Random();
			tiempo_respuesta = r * 3;
			sigma = tiempo_respuesta;
			id = *(double*)(x.value);
			estado_controlador = true;
			proc_entrada = true;
			evento_actual = eventos(id, x.port);
			printLog("Controlador dext: Procesando evento de entrada: ID = %f, PUERTO = %d. Tarda en responder: %f\n", id, x.port, sigma);
		}
	}
	if (x.port == 1) {
		if (estado_controlador || proc_salida) {
			printLog("Controlador dext: Se agrego evento pendiente: ID = %f, PUERTO = %d\n", *(double*)(x.value), x.port);
			sigma -= e;
			eventos_pendientes.push_back(eventos(*(double*)(x.value), x.port));
		} else if (!proc_salida) {
			r = rng.Random();
			tiempo_respuesta = r * 3;
			sigma = tiempo_respuesta;
			id = *(double*)(x.value);
			estado_controlador = true;
			proc_salida = true;
			evento_actual = eventos(id, x.port);
			printLog("Controlador dext: Procesando evento de salida: ID = %f, PUERTO = %d. Tarda en responder: %f\n", id, x.port, sigma);
		}
	}
	if (x.port == 2) {
		proc_entrada = false;
		c += 1;
		ingreso = true;
		sigma = 0.0;
		id = *(double*)(x.value);
		printLog("Controlador dext: Vino un evento por el puerto %d con el ID = %f. Se incrementa la cantidad de vehiculos a %f\n", x.port, id, c);
	}
	if (x.port == 3) {
		proc_salida = false;
		c -= 1;
		egreso = true;
		sigma = 0.0;
		id = *(double*)(x.value);
		printLog("Controlador dext: Vino un evento por el puerto %d con el ID = %f. Se decrementa la cantidad de vehiculos a %f\n", x.port, id, c);
	}
	printLog("Controlador dext: FIN | t=%f | sigma=%f | ec=%s | pe=%s | ps=%s | eventos pendientes=%zu\n", t, sigma, estado_controlador ? "true" : "false", proc_entrada ? "true" : "false", proc_salida ? "true" : "false", eventos_pendientes.size());
}
Event controlador::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
	if (ingreso) {
		printLog("Controlador: Se ingreso el vehiculo ID = %f al estacionamiento en t = %f \n", id, t);
		ingreso = false;
		return Event(&id, 3);
	} else if (egreso) {
		printLog("Controlador: Se egreso el vehiculo ID = %f del estacionamiento en t = %f \n", id, t);
		egreso = false;
		return Event(&id, 4);
	} else if (proc_salida && evento_actual.puerto == 1) {
		printLog("Controlador: Se permitio la salida del ID = %f en t = %f \n", evento_actual.id, t);
		return Event(&evento_actual.id, 2);
	} else if (proc_entrada && evento_actual.puerto == 0) {
		printLog("Controlador: Se permitio la entrada de ID = %f en t = %f \n", evento_actual.id, t);
		return Event(&evento_actual.id, 0);
	} else {
		printLog("Controlador: Se denego la entrada ID = %f en t = %f \n", evento_actual.id, t);
		return Event(&evento_actual.id, 1);
	}
}
void controlador::exit() {
//Code executed at the end of the simulation.
}
