#include <iostream.h>
#include <stdio.h>
#include "init.h"
#include "pcb.h"
#include "schedule.h"
#include "lst_slp.h"

// Declarations for functions
extern int retVal;
int carg;
char** varg;

int main(int argc, char* argv[])
{
	// Prepare params
	carg = argc;
	varg = argv;

	// Prepare thread(s) to run
	init();

	// Do something
	doSomething();

	// Restore initial state of hardware
	restore();

	return retVal;
}
