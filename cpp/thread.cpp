
#include <stdio.h>
#include "thread.h"
#include "pcb.h"
#include "schedule.h"
#include "init.h"
#include "lst_slp.h"

LST_SLEEP lst_sleeping;

// Protected constructor
Thread::Thread(StackSize stackSize, Time timeSlice)
{
	#ifndef BCC_BLOCK_IGNORE
	stack_lock;
	#endif
	myPCB = new PCB( this, stackSize, timeSlice );
	#ifndef BCC_BLOCK_IGNORE
	stack_unlock;
	#endif
}

// Put thread into the scheduler
void Thread::start()
{
	#ifndef BCC_BLOCK_IGNORE
	stack_lock
	#endif
	Scheduler::put( this->myPCB );
	#ifndef BCC_BLOCK_IGNORE
	stack_unlock
	#endif
}

// STATIC: Put running thread to sleep, for timeToSleep timer ticks
void Thread::sleep(Time timeToSleep)
{
	#ifndef BCC_BLOCK_IGNORE
	stack_lock
	#endif
	PCB::running->setSleep();
	lst_sleeping.put( PCB::running, timeToSleep );
	dispatch();
	#ifndef BCC_BLOCK_IGNORE
	stack_unlock
	#endif
}

void Thread::waitToComplete()
{
	#ifndef BCC_BLOCK_IGNORE
	stack_lock;
	#endif
	if ( myPCB->isFinished() == 0 )
	{
		// Add a queue, maybe?
		this->myPCB->blockThread( PCB::running );
		PCB::running->setBlocked();
		dispatch();
	};
	#ifndef BCC_BLOCK_IGNORE
	stack_unlock
	#endif
}

// Destructor for some thread
Thread::~Thread() {  delete myPCB; }



