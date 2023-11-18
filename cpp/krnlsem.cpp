
#include <stdio.h>
#include "init.h"
#include "pcb.h"
#include "krnlsem.h"
#include "schedule.h"
#include "lst_fifo.h"

int retValue;

// Create kernel semaphore
KernelSem::KernelSem(int init) : value(init), blocked() {}

// Destroy kernel semaphore
KernelSem::~KernelSem() { blocked.putAllInScheduler(); }

// Wait operation on the semaphore
int KernelSem::wait(int toBlock)
{
#ifndef BCC_BLOCK_IGNORE
	stack_lock
#endif
	// We are doing ordinary operation => We need to block the running thread
	if ( toBlock != 0 )
	{
		if ( value-- <= 0 )	// Block running thread
		{
			PCB::running->setBlocked();
			blocked.put( PCB::running );
			dispatch();
			PCB::running->rstBlocked();
			retValue = 1;
		} else retValue = 0;
	} else	// toBlock == 0
	{
		// We don't need to block the semaphore
		if ( value <= 0 )
		{
			retValue = -1;
		} else 	// value>0
		{
			--value;
			retValue = 0;
		};
	};
#ifndef BCC_BLOCK_IGNORE
	stack_unlock
#endif
	return retValue;
}

int KernelSem::val() const { return value; }

// Signal operation on the semaphore
void KernelSem::signal()
{
	#ifndef BCC_BLOCK_IGNORE
	stack_lock;
	#endif
	// Deblock one process & put it in ready queue
	if ( value++ < 0 )
	{
		Scheduler::put( blocked.getFirst() );
	};
	#ifndef BCC_BLOCK_IGNORE
	stack_unlock;
	#endif
}


