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
	if (x.port == 0 && c < 40.0 and !b) {
		b = true;
		int tiempo_respuesta = rng.IRandomX(0,3);
		while (tiempo_respuesta == 0) {
			tiempo_respuesta = rng.IRandomX(0,3);
		}
		printLog("Controlador: Hay lugar. PE libre. El controlador tarda en responder: %d \n", tiempo_respuesta);
		sigma = tiempo_respuesta;
	}else if(x.port == 0 && c >= 40.0) {
		b = false;
		int tiempo_respuesta = rng.IRandomX(0,3);
		while (tiempo_respuesta == 0) {
			tiempo_respuesta = rng.IRandomX(0,3);
		}
		printLog("Controlador: No hay lugar. El controlador tarda en responder: %d \n", tiempo_respuesta);
		sigma = tiempo_respuesta;
	}else if(x.port == 1 && !b2) {
		b2 = true;
		int tiempo_respuesta = rng.IRandomX(0,3);
		while (tiempo_respuesta == 0) {
			tiempo_respuesta = rng.IRandomX(0,3);
		}
		printLog("Controlador: Vehiculo quiere salir. El controlador tarda en responder: %d \n", tiempo_respuesta);
		sigma = tiempo_respuesta;
	}else if(x.port == 2) {
		b = false;
		c += 1.0;
		sigma = infinity;
	}else if(x.port == 3) {
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
	if (b2) {
		printLog("Controlador: Se permitio la salida en t = %f \n", t);
		permitirSalida = 2.0;
		return Event(&permitirSalida, 2);
	} else if (b) {
		printLog("Controlador: Se permitio la entrada en t = %f \n", t);
		printLog("Vehiculo ingresando al estacionamiento en tiempo t = %f \n", t);
		permitirEntrada = 0.0;
		return Event(&permitirEntrada,0);
	} else {
		printLog("Controlador: Se denego la entrada en t = %f \n", t);
		denegarEntrada = 1.0;
		return Event(&denegarEntrada, 1);
	}
}
void controlador::exit() {
//Code executed at the end of the simulation.
printf("salida del controlador");
}
