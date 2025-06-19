#include "barrera_entrada.h"

const double barrera_entrada::infinity = std::numeric_limits<double>::infinity();

void barrera_entrada::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type

lo = false;
sigma = infinity;
}

double barrera_entrada::ta(double t) {
//This function returns a double.
return sigma;
}

void barrera_entrada::dint(double t) {
lo = false;
sigma = infinity;
}

void barrera_entrada::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
if (x.port == 0){
	lo = true;
	sigma = 4 + 2 + 4; // cambiar por 2 por una uniforme(1,3)
}else if (x.port == 1) {
	lo = false;
	sigma = 2; // cambiar por 2 por una uniforme(0,2)
}

}

Event barrera_entrada::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)

	static double finBarrera = 1.0;

if (lo) {
	static double vehiculoIngreso = 1.0;
	std::queue<Event> eventos;
	eventos.push(Event(&vehiculoIngreso, 0)); // puerto 0 para vehiculoIngreso
	eventos.push(Event(&finBarrera, 1)); // puerto 1 para fin de barrera
	return eventos;
}else {
	return Event(&finBarrera, 1); // puerto 1 para fin de barrera
}

}

void barrera_entrada::exit() {
//Code executed at the end of the simulation.
printf("Finalizando barrera_entrada...\n");

}
