
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include "thread.h"
#include "lst_fifo.h"
#include "pcb.h"
#include "init.h"
#include "schedule.h"

unsigned int hss, hsp, hbp;

// Create PCB for new process
PCB::PCB(Thread* thread, unsigned int ss, unsigned int timeSlice)
{
	#ifndef BCC_BLOCK_IGNORE
	stack_lock;
	#endif

	// Connect thread with PCB
	myThread = thread;

	// Allocate new stack for this thread
	this->stack = new unsigned int[ss];

	// Initiate start values for stack
	stack[ss-1] = 0x0200;
	#ifndef BCC_BLOCK_IGNORE
	stack[ss-2] = FP_SEG( PCB::wrapper );
	stack[ss-3] = FP_OFF( PCB::wrapper );
	#endif

	// Allow processor to run thread for (un)limited time
	this->time = timeSlice;

	#ifndef BCC_BLOCK_IGNORE
	hss = FP_SEG( stack + ss - 12 );
	hsp = FP_OFF( stack + ss - 12 );
	hbp = FP_OFF( stack + ss - 12 );
	#endif
	this->ss = hss;
	this->sp = hsp;
	this->bp = hbp;

	// Set thread NOT to be finished & not idle
	this->is_finished = 0;

	// Set thread to be active
	this->is_sleeping = 0;

	// Set thread not to be blocked
	this->is_blocked = 0;

	#ifndef BCC_BLOCK_IGNORE
	stack_unlock;
	#endif
}

// Create PCB for special reasons
PCB::PCB(unsigned int timeSlice)
{
	#ifndef BCC_BLOCK_IGNORE
	stack_lock;
	#endif
	this->ss = 0;
	this->sp = 0;
	this->bp = 0;
	this->time = timeSlice;
	this->stack = 0;
	this->myThread = 0;
	this->is_finished = 0;
	this->is_sleeping = 0;
	this->is_blocked = 0;
	#ifndef BCC_BLOCK_IGNORE
	stack_unlock;
	#endif
}

PCB::PCB(void (*f)()) : myThread(0), time(5)
{
	#ifndef BCC_BLOCK_IGNORE
	stack_lock;
	#endif
	// Allocate new stack for this thread
	this->stack = new unsigned int[24];

	// Initiate start values for stack
	stack[ss-1] = 0x0200;
	#ifndef BCC_BLOCK_IGNORE
	stack[ss-2] = FP_SEG( f );
	stack[ss-3] = FP_OFF( f );
	#endif

	#ifndef BCC_BLOCK_IGNORE
	hss = FP_SEG( stack + ss - 12 );
	hsp = FP_OFF( stack + ss - 12 );
	hbp = FP_OFF( stack + ss - 12 );
	#endif
	this->ss = hss;
	this->sp = hsp;
	this->bp = hbp;

	// Set thread NOT to be finished & not idle
	this->is_finished = 0;

	// Set thread to be active
	this->is_sleeping = 0;

	// Set thread not to be blocked
	this->is_blocked = 0;
	#ifndef BCC_BLOCK_IGNORE
	stack_unlock;
	#endif
}

// Manage the wrapper around the RUN thread function
void PCB::wrapper()
{
	// Execute thread's body
	PCB::running->myThread->run();

	// Thread has completed it's work, prepare for closure

	#ifndef BCC_BLOCK_IGNORE
	stack_lock;
	#endif
	// 1. Set thread to be finished
	PCB::running->setFinished();

	// 2. Deblock parent thread which called destructor
	if ( PCB::running->blocked.elements() != 0 )
		PCB::running->unblockThreads();

	// 3. Change context explicitly
	dispatch();
}

// Block thread
void PCB::blockThread(PCB* p)
{
	#ifndef BCC_BLOCK_IGNORE
	stack_lock;
	#endif
	blocked.put(p);
	#ifndef BCC_BLOCK_IGNORE
	stack_unlock;
	#endif
}

// Unblock all threads
void PCB::unblockThreads()
{
	#ifndef BCC_BLOCK_IGNORE
	stack_lock;
	#endif
	blocked.putAllInScheduler();
	#ifndef BCC_BLOCK_IGNORE
	stack_unlock;
	#endif
}


// GET METHODS
// ================================================
unsigned int PCB::getSP() { return sp; }
unsigned int PCB::getSS() { return ss; }
unsigned int PCB::getBP() { return bp; }
unsigned int PCB::getTime() { return time; }
unsigned int PCB::isFinished() { return is_finished; }
unsigned int PCB::isSleeping() { return is_sleeping; }
unsigned int PCB::isBlocked() { return is_blocked; }
unsigned int PCB::isReady()
{
	return PCB::running->isFinished()==0 &&
		   PCB::running->isSleeping()==0 &&
		   PCB::running->isBlocked()==0;
}


// SET METHODS
// ================================================
void PCB::setSS(unsigned int ss) { this->ss = ss; }
void PCB::setSP(unsigned int sp) { this->sp = sp; }
void PCB::setBP(unsigned int bp) { this->bp = bp; }
void PCB::setFinished() { this->is_finished = 1; }
void PCB::clearStack() { delete stack; }
void PCB::setSleep() { this->is_sleeping = 1; }
void PCB::rstSleep() { this->is_sleeping = 0; }
void PCB::setBlocked() { this->is_blocked = 1; }
void PCB::rstBlocked() { this->is_blocked = 0; }


// Destroy PCB's data
PCB::~PCB()
{
	// Delete thread
	this->myThread = 0;

	// Remove stack
	if (this->stack != 0) delete this->stack;
}

// Set running to NULL
PCB* PCB::running = 0;
PCB* PCB::main    = 0;
PCB* PCB::wait    = 0;
