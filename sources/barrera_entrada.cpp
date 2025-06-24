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
}

double barrera_entrada::ta(double t) {
//This function returns a double.
	return sigma;
}

void barrera_entrada::dint(double t) {
	b = false;
	sigma = infinity;
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
		printLog("Permitiendo entrada... el auto tarda en cruzar: %d \n", tiempo_entrada);
		printLog("Tiempo actual: %f \n", t);
		sigma = 4 + tiempo_entrada + 4;
	} else if (x.port == 1) {
		b = false;
		int tiempo_salida = rng.IRandomX(0, 2);
		while (tiempo_salida == 0) {
			tiempo_salida = rng.IRandomX(0, 2);
		}
		printLog("Rechazando entrada... el auto tarda en irse: %d \n", tiempo_salida);
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
		vehiculoIngreso = 1.0;
		salidas[0] = (Event(&vehiculoIngreso, 0)); // puerto 0 para vehiculoIngreso
		salidas[1] = (Event(&finBarrera, 1)); // puerto 0 para fin de barrera
		printLog("Vehiculo ingresando a la barrera...\n");
		printLog("Contenido del arreglo salidas:\n");
		for (int i = 0; i < 2; ++i) {
			double* val = static_cast<double*>(salidas[i].value);
			printLog("salidas[%d]: value=%f\n", i, val ? *val : -1.0);
		}
		return Event(&salidas, 0);
	}else {
		printLog("La barrera termino...\n");
		return Event(&finBarrera, 1); // puerto 1 para fin de barrera
	}
}

void barrera_entrada::exit() {
//Code executed at the end of the simulation.
	printf("Finalizando barrera_entrada...\n");
}
