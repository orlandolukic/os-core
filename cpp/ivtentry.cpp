
#include <stdio.h>
#include "init.h"
#include "ivtentry.h"
#include "kernelEv.h"

// Define static data
IVTEntry* IVTEntry::interruptRoutines[256] = {0};

IVTEntry::IVTEntry(unsigned char ivtNo, farPointer routine)
{
	// Initialize event in Kernel
	// ==============================
	#ifndef BCC_BLOCK_IGNORE
	stack_lock;
	#endif

	// Set number of interrupt
	this->no = ivtNo;

	// Redirect this IVTEntry to interruptRoutines IVTEntry
	interruptRoutines[ivtNo] = this;

	// Put new routine
	this->newRoutine = routine;

	#ifndef BCC_BLOCK_IGNORE
	stack_unlock;
	#endif
}

// Signaling operation when interrupt occurred
void IVTEntry::signal()
{
	if ( KernelEv::oldInterruptRoutines[no] )
		KernelEv::oldInterruptRoutines[no]->signal();
}

// Call old routine
void IVTEntry::callOldRoutine()
{
	if ( KernelEv::oldInterruptRoutines[no] )
		KernelEv::oldInterruptRoutines[no]->oldRoutine();
}

// Destructor
IVTEntry::~IVTEntry()
{
	#ifndef BCC_BLOCK_IGNORE
	stack_lock;
	#endif
	// Empty entry in routine's array
	interruptRoutines[no] = 0;
	#ifndef BCC_BLOCK_IGNORE
	stack_unlock;
	#endif
}

