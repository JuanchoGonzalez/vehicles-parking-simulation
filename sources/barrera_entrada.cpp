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
	apertura_barrera_entrada = va_arg(parameters,double);
	cierre_barrera_entrada = va_arg(parameters,double);
	min_cruce_e = va_arg(parameters,double);
	max_cruce_e = va_arg(parameters,double);
	min_salida_e = va_arg(parameters,double);
	max_salida_e = va_arg(parameters,double);
	proc_barrera_e = false;
	sigma = infinity;
}
double barrera_entrada::ta(double t) {
//This function returns a double.
	return sigma;
}
void barrera_entrada::dint(double t) {
	proc_barrera_e = false;
	sigma = infinity;
}
void barrera_entrada::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
	if (x.port == 0){ // se permite la entrada (viene el id del controlador)
		proc_barrera_e = true;
		r = rng.Random();
		cruce_vehiculo_e = min_cruce_e + r * (max_cruce_e - min_cruce_e); // formula inversa uniforme 1 + r * (3 - 1)
		id = *(double*)(x.value);
		sigma = apertura_barrera_entrada + cruce_vehiculo_e + cierre_barrera_entrada;
		printLog("Barrera Entrada dext: se permitio la entrada de vehiculo ID = %f. Deberia entrar en: %f + %f + %f + %f\n", id, t, apertura_barrera_entrada, cruce_vehiculo_e, cierre_barrera_entrada);
	} else if (x.port == 1) { // se deniega la entrada (viene el id del controlador)
		proc_barrera_e = false;
		r = rng.Random();
		sigma = min_salida_e + r * (max_salida_e - min_salida_e); // formula inversa uniforme 0 + r * (2 - 0)
		printLog("Barrera Entrada dext: se rechazo la entrada de vehiculo ID = %f. Deberia irse en: %f + %f\n", *(double*)(x.value) , t, sigma);
	}
}

Event barrera_entrada::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
	if (proc_barrera_e) {
		printLog("Barrera Entrada lambda: como se permitio la entrada, el vehiculo con ID = %f ingreso en t = %f \n", id, t);
		return Event(&id, 0);
	} else {
		printLog("Barrera Entrada lambda: como se denego la entrada, se fue en t = %f \n", t);
		id = -1; // Cambia el id que viene a -1 como bandera que se denego la entrada
		return Event(&id, 1);
	}
}

void barrera_entrada::exit() {
//Code executed at the end of the simulation.
}
