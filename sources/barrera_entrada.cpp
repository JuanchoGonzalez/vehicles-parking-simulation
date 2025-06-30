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
	proc_barrera_e = false;
	sigma = infinity;
}
double barrera_entrada::ta(double t) {
//This function returns a double.
	return sigma;
}
void barrera_entrada::dint(double t) {
	printLog("Barrera Entrada dint: INICIO | t=%f | proc_barrera_e=%d | sigma=%f\n", t, proc_barrera_e, sigma);
	proc_barrera_e = false;
	sigma = infinity;
	printLog("Barrera Entrada dint: FIN | t=%f | proc_barrera_e=%d | sigma=%f\n", t, proc_barrera_e, sigma);
	// estos valores xq el vehiculo pasa de uno en uno.
}
void barrera_entrada::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
    printLog("Barrera Entrada dext: INICIO | t=%f | proc_barrera_e=%d | sigma=%f | port=%d\n", t, proc_barrera_e, sigma, x.port);
	if (x.port == 0){ // se permite la entrada (viene el id del controlador)
		proc_barrera_e = true;
		r = rng.Random();
		cruce_vehiculo_e = 1 + r * (3 - 1); 
		id = *(double*)(x.value);
		sigma = 4 + cruce_vehiculo_e + 4;
		printLog("Barrera Entrada dext: se permitio la entrada de vehiculo ID = %f. Deberia entrar en: %f + 4 + %f + 4\n", id, t, cruce_vehiculo_e);
	} else if (x.port == 1) { // se deniega la entrada pq no hay lugar (viene el id del controlador)
		proc_barrera_e = false;
		r = rng.Random();
		salida_vehiculo_e = r * 2; // formula inversa uniforme 0 + r * (2 - 0)
		sigma = salida_vehiculo_e;
		printLog("Barrera Entrada dext: se rechazo la entrada de vehiculo ID = %f. Deberia irse en: %f + %f\n", id, t, salida_vehiculo_e);
	}
	printLog("Barrera Entrada dext: FIN | t=%f | proc_barrera_e=%d | sigma=%f | port=%d\n", t, proc_barrera_e, sigma, x.port);
}

Event barrera_entrada::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
	if (proc_barrera_e) {
		printLog("Barrera Entrada lambda: como se permitio la entrada, el vehiculo con ID = %f ingreso en t = %f \n", id, t);
	} else {
		printLog("Barrera Entrada lambda: como se denego la entrada, el vehiculo con ID = %f se fue en t = %f \n", id, t);
	}
	return Event(&id, 0);
	// else {
	// 	printLog("Barrera Entrada: como se rechazo la entrada, el vehiculo se fue en t = %f \n", t);
	// 	return Event(&id, 1); // puerto 1 para fin de barrera
	// }
	// VER PARA EL MONITOR QUE SACAR !!!!!!!
}

void barrera_entrada::exit() {
//Code executed at the end of the simulation.
}
