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
	b=false;
	b2=false;
	c=0;
	sigma=infinity;
	ingreso=false;
	id = &id_actual;
	id2 = &id_salida;
}
double controlador::ta(double t) {
//This function returns a double.
	return sigma;
}
void controlador::dint(double t) {
	sigma = infinity;
}
void controlador::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
	if (x.port == 0 && c < 30.0 && !b) { // llega por el puerto 0. hay lugar. pe libre
		printLog("DE SENSOR A CONTROLADOR, VALOR = %f\n", *static_cast<double*>(x.value));
		b = true;
		double r = rng.Random();
		double tiempo_respuesta = 0 + r * (3 - 0);
		sigma = tiempo_respuesta;
		id_actual = *static_cast<double*>(x.value);
		printLog("Controlador: Hay lugar. PE libre. El controlador tarda en responder: %d . ID: %f\n", tiempo_respuesta, *id);
	} else if (x.port == 0 && c >= 30.0) { // lega por el puerto 0. no hay lugar. no nos interesa como esta el pe
		b = false;
		double r = rng.Random();
		double tiempo_respuesta = 0 + r * (3 - 0);
		sigma = tiempo_respuesta;
		id_actual = *static_cast<double*>(x.value);
		printLog("Controlador: No hay lugar. El controlador tarda en responder: %d \n", tiempo_respuesta);
	} else if (x.port == 1 && !b2) { // llega por el puerto 1. el ps esta libre
		b2 = true;
		double r = rng.Random();
		double tiempo_respuesta = 0 + r * (3 - 0);
		sigma = tiempo_respuesta;
		id_salida = *static_cast<double*>(x.value);
		printLog("Controlador: Vehiculo quiere salir. El controlador tarda en responder: %d \n", tiempo_respuesta);
	} else if (x.port == 2) { // vehiculo ingresando al estacionamiento
		b = false;
		c += 1.0;
		sigma = 0.0;
		ingreso = true;
	} else if (x.port == 3) { // vehiculo saliendo del estacionamiento
		b2 = false;
		c -= 1.0;
		sigma = infinity;
	}
}
Event controlador::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
	if (ingreso) {
		printLog("Controlador: Se ingreso el vehiculo ID = %f al estacionamiento en t = %f \n", *id, t);
		ingreso = false;
		vehiculoIngresando = id;
		return Event(id, 3);
	} else if (b) {
		printLog("Controlador: Se permitio la entrada de ID = %f en t = %f \n", *id, t);
		permitirEntrada = id;
		return Event(permitirEntrada, 0);
	} else if (b2) {
		printLog("Controlador: Se permitio la salida del ID = %f en t = %f \n", *id2, t);
		permitirSalida = id2;
		return Event(permitirSalida, 2);
	} else {
		printLog("Controlador: Se denego la entrada ID = %f en t = %f \n", *id, t);
		denegarEntrada = id;
		return Event(denegarEntrada, 1);
	}
}
void controlador::exit() {
//Code executed at the end of the simulation.
}
