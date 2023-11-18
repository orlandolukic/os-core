
#include "init.h"
#include <dos.h>
#include "pcb.h"
#include "ivtentry.h"
#include "kernelEv.h"
#include "schedule.h"

farPointer tempPointer;

// Initialize static data
KernelEv* KernelEv::oldInterruptRoutines[256] = {0};

KernelEv::KernelEv(unsigned char ivtNo, Event* event)
{
	#ifndef BCC_BLOCK_IGNORE
	stack_lock;
	#endif
	// Set creator to be current running thread
	creator = PCB::running;

	// Set number of the interrupt
	this->ivtNo = ivtNo;

	// Set current value of the semaphore
	this->value = 0;

	// Connect event with KernelEv
	this->ev = event;

	// Set static data
	oldInterruptRoutines[ivtNo] = this;

	// Set old interrupt routine
	#ifndef BCC_BLOCK_IGNORE
	tempPointer = getvect(ivtNo);
	#endif
	oldRoutine = tempPointer;

	// Override old interrupt routine with new one
	#ifndef BCC_BLOCK_IGNORE
	setvect( ivtNo, IVTEntry::interruptRoutines[ivtNo]->newRoutine );
	#endif

	#ifndef BCC_BLOCK_IGNORE
	stack_unlock;
	#endif
}

// Destroy KernelEvent
KernelEv::~KernelEv()
{
	// Put creator to null
	creator = 0;

	// Reset static data
	oldInterruptRoutines[ivtNo] = 0;

	// Put event to null
	ev = 0;

	// Restore old interrupt routine
	#ifndef BCC_BLOCK_IGNORE
	setvect( ivtNo, oldRoutine );
	#endif
}

void KernelEv::wait()
{

	#ifndef BCC_BLOCK_IGNORE
	stack_lock;
	#endif

	if ( PCB::running == this->creator )
	{
		if ( value-- == 0 )
		{
			PCB::running->setBlocked();
			dispatch();
		} else value = 0;
	};

	#ifndef BCC_BLOCK_IGNORE
	stack_unlock;
	#endif

}

// Signal operation on the semaphore
void KernelEv::signal()
{

	#ifndef BCC_BLOCK_IGNORE
	stack_lock;
	#endif

	value = 0;
	this->creator->rstBlocked();
	Scheduler::put( this->creator );
	dispatch();

	#ifndef BCC_BLOCK_IGNORE
	stack_unlock;
	#endif

}
