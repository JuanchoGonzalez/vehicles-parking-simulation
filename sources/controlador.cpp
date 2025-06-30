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
	proc_entrada=false;
	proc_salida=false;
	ingreso=false;
	c=0;
	sigma=infinity;
}

double controlador::ta(double t) {
//This function returns a double.
	return sigma;
}
void controlador::dint(double t) {
	if (proc_entrada && proc_salida) {
		sigma = infinity;
	} else if (!l_salida.empty() && !proc_salida) {
		vehiculo = l_salida.front();
		l_salida.pop_front();
		proc_salida = true;
		r = rng.Random();
		tiempo_respuesta = r * 3; // formula inversa uniforme 0 + r * (3 - 0)
		sigma = tiempo_respuesta;
		id_salida = vehiculo.first;
		printLog("Controlador: Salio de la cola ID: %f para salir. pe = %d. ps = %d.\n", vehiculo.first, proc_entrada, proc_salida);
	} else if (!l_entrada.empty() && !proc_entrada) {
		vehiculo = l_entrada.front();
		l_entrada.pop_front();
		proc_entrada = true;
		r = rng.Random();
		tiempo_respuesta = r * 3; // formula inversa uniforme 0 + r * (3 - 0)
		sigma = tiempo_respuesta;
		id_entrada = vehiculo.first;
		printLog("Controlador: Salio de la cola ID: %f para entrar. pe = %d. ps = %d.\n", vehiculo.first, proc_entrada, proc_salida);
	} else {
		sigma = infinity;
	}
}
void controlador::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
	printLog("Controlador dext: INICIO | t=%f | sigma=%f | proceso de entrada=%d | proceso de salida=%d\n", t, sigma, proc_entrada, proc_salida);

	if (x.port == 0) { // llego un auto desde el sensor de entrada
		if (c >= 30.0) { // no hay lugar
			r = rng.Random();
			tiempo_respuesta = r * 3;  // formula inversa uniforme 0 + r * (3 - 0)
			sigma = tiempo_respuesta;
			printLog("Controlador: No hay lugar. El controlador tarda en responder: %f \n", tiempo_respuesta);
		} else if (!proc_entrada) { // hay lugar. proceso de entrada libre
			proc_entrada = true;
			r = rng.Random();
			tiempo_respuesta = r * 3;
			sigma = tiempo_respuesta;
			id_entrada = *(double*)(x.value);
			printLog("DE SENSOR A CONTROLADOR, VALOR = %f\n", id_entrada);
			printLog("Controlador: Hay lugar. El controlador tarda en responder: %f\n", tiempo_respuesta);
		} else { // hay lugar. proceso de entrada ocupado
			id_cola_entrada = *(double*)(x.value);
			puerto_cola = x.port;
			vehiculo = std::make_pair(id_cola_entrada, puerto_cola);
			l_entrada.push_back(vehiculo);
            printLog("Controlador: Lista de espera (l):\n");
            for (size_t i = 0; i < l_entrada.size(); ++i) {
                printLog("%f, %f\n", l_entrada[i].first, l_entrada[i].second);
            }
            printLog("\n");
			sigma -= e;
		}
	} else if (x.port == 1) {
		if (!proc_salida) {
			proc_salida= true;
			r = rng.Random();
			tiempo_respuesta = 0 + r * (3 - 0);
			sigma = tiempo_respuesta;
			id_salida = *(double*)(x.value);
			printLog("Controlador: Vehiculo quiere salir. El controlador tarda en responder: %f \n", tiempo_respuesta);
		} else {
			id_cola_salida = *(double*)(x.value);
			puerto_cola = x.port;
			vehiculo = std::make_pair(id_cola_salida, puerto_cola);
			l_salida.push_back(vehiculo);
            printLog("Controlador: Lista de espera (l):\n");
            for (size_t i = 0; i < l_salida.size(); ++i) {
                printLog("ID:%f, PUERTO:%f\n", l_salida[i].first, l_salida[i].second);
            }
            printLog("\n");
			sigma -= e;
		}
	} else if (x.port == 2) { // vehiculo ingresando al estacionamiento
		id_entrada = *(double*)(x.value);
		proc_entrada = false;
		c += 1.0;
		sigma = 0.0;
		ingreso = true;
	} else if (x.port == 3) { // vehiculo saliendo del estacionamiento
		proc_salida = false;
		c -= 1.0;
    	sigma = infinity;
	}
	printLog("Controlador dext: FIN | t=%f | sigma=%f | proceso de entrada=%d | proceso de salida=%d\n", t, sigma, proc_entrada, proc_salida);
}
Event controlador::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
	if (ingreso) {
		printLog("Controlador: Se ingreso el vehiculo ID = %f al estacionamiento en t = %f \n", id_entrada, t);
		ingreso = false;
		return Event(&id_entrada, 3);
	} else if (proc_salida) {
		printLog("Controlador: Se permitio la salida del ID = %f en t = %f \n", id_salida, t);
		return Event(&id_salida, 2);
	} else if (proc_entrada) {
		printLog("Controlador: Se permitio la entrada de ID = %f en t = %f \n", id_entrada, t);
		return Event(&id_entrada, 0);
	} else {
		printLog("Controlador: Se denego la entrada ID = %f en t = %f \n", id_entrada, t);
		return Event(&id_entrada, 1);
	}
}
void controlador::exit() {
//Code executed at the end of the simulation.
}
