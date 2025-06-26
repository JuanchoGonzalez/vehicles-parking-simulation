#include "barrera_entrada.h"
#include "../random/lib/randomc.h"
#include "../random/lib/stocc.h"

const double barrera_entrada::infinity = std::numeric_limits<double>::infinity();

void barrera_entrada::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
	va_list parameters;
	va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
	b = false;
	sigma = infinity;
	id = &id_actual;
}

double barrera_entrada::ta(double t) {
//This function returns a double.
	return sigma;
}

void barrera_entrada::dint(double t) {
	b = false;
	sigma = infinity;

	// estos valores xq el vehiculo pasa de uno en uno.
}

void barrera_entrada::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
	if (x.port == 0){
		b = true;
		int tiempo_entrada = rng.IRandomX(1, 3);
		printLog("x.value = %f\n", *static_cast<double*>(x.value));
		id_actual = *static_cast<double*>(x.value);
		printLog("Barrera Entrada: se permitio la entrada de vehiculo ID = %f, el auto tarda en cruzar %d unidades. Deberia entrar en: %f + 4 + %d + 4\n", id_actual, tiempo_entrada, t, tiempo_entrada);
		sigma = 4 + tiempo_entrada + 4;
	} else if (x.port == 1) {
		b = false;
		int tiempo_salida = rng.IRandomX(0, 2);
		while (tiempo_salida == 0) {
			tiempo_salida = rng.IRandomX(0, 2);
		}
		id = static_cast<double*>(x.value);
		printLog("Barrera Entrada: se rechazo la entrada de vehiculo ID = %f, el auto tarda en irse %d unidades. Tiempo actual: %f \n",*id,  tiempo_salida, t);
		sigma = tiempo_salida;
	}
}

Event barrera_entrada::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
	finBarrera = 1.0;

	if (b) {
		vehiculoIngreso = &id_actual;
		salidas = std::make_pair(vehiculoIngreso, finBarrera);
		printLog("Barrera Entrada: como se permitio la entrada, el vehiculo con ID = %f ingreso en t = %f \n",id_actual, t);
		printLog("	Contenido de la tupla salidas:\n");
		printLog("	salidas.first: vehiculoIngreso = %f\n", *salidas.first);
		printLog("	salidas.second: finBarrera =%f\n", salidas.second);
		return Event(&salidas, 0);
	}else {
		printLog("Barrera Entrada: como se rechazo la entrada, el vehiculo se fue en t = %f \n", t);
		return Event(&finBarrera, 1); // puerto 1 para fin de barrera
	}
}

void barrera_entrada::exit() {
//Code executed at the end of the simulation.
	printf("Finalizando barrera_entrada...\n");
}
