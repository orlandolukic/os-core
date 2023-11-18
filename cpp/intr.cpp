#include <stdlib.h>
#include <stdio.h>
#include "init.h"
#include "pcb.h"
#include "schedule.h"
#include "lst_fifo.h"

// Global var(s)
volatile int timer = 1, context_switch = 0, is_timer_ready = 0, context_switch_happened = 0, allowContextSwitch = 1, preempt = 0;
unsigned int sss = 10, ssp = 10, sbp, putInQueue = 0;

// Timer Interrupt Routine
void interrupt TimerIntr()
{
	// Update time for sleeping threads
	if ( preempt == 0 )
		update_sleeping_threads();

	// Check if timer is setup
	if ( is_timer_ready )
	{
		if (PCB::running != 0 && PCB::running->getTime() != 0 && timer > 0 ) timer--;

		// Contect switch if necessary
		context_switch_happened =
				context_switch ||
				( PCB::running != 0 && timer == 0 && PCB::running->getTime() != 0 ) ||
				( timer == 0 && PCB::running == 0 ) ||
				( putInQueue==1 && PCB::running == PCB::wait );
		context_switch_happened = context_switch_happened && allowContextSwitch;

		if ( context_switch_happened )
		{
			// If request happened, be sure to clear it
			context_switch = 0;

			#ifndef BCC_BLOCK_IGNORE
			asm {
				mov sss, ss
				mov ssp, sp
				mov sbp, bp
			}
			#endif

			// If running exists => SAVE RUNNING
			if ( PCB::running != 0 )
			{
				PCB::running->setSS( sss );
				PCB::running->setSP( ssp );
				PCB::running->setBP( sbp );

			} else	// SAVE MAIN || WAIT
			{
				if ( PCB::main->isBlocked() )	// if MAIN thread is blocked, save WAIT thread
				{
					PCB::wait->setSS( sss );
					PCB::wait->setSP( ssp );
					PCB::wait->setBP( sbp );
				} else
				{
					PCB::main->setSS( sss );
					PCB::main->setSP( ssp );
					PCB::main->setBP( sbp );
				};
			};

			// Condition when to put thread in the queue
			if (
					PCB::running != 0 &&
					PCB::running != PCB::main &&
					PCB::running != PCB::wait &&
					PCB::running->isReady()
				)
				Scheduler::put( PCB::running );

				// Get one active thread
				PCB::running = Scheduler::get();

				// If we didn't catch anything from the queue, set running to PCB::wait
				if ( PCB::running == 0 )
				{
					if ( PCB::main->isBlocked() )
						PCB::running = PCB::wait;
					else
						PCB::running = PCB::main;
				};

			sss = PCB::running->getSS();
			ssp = PCB::running->getSP();
			sbp = PCB::running->getBP();

			// Set quantum to current thread
			timer = PCB::running->getTime();

			// Fill processor with content
			#ifndef BCC_BLOCK_IGNORE
			asm {
				mov ss, sss
				mov sp, ssp
				mov bp, sbp
			}
			#endif
		};	// /Context switch if necessary
	}; // if ( is_timer_ready ) ...

	// Do system stuff...
	if ( preempt == 0 )	asm int 60h;

	// Notify test-environment for new timer tick
	if ( preempt == 0 && allowContextSwitch==1 ) tick();

	// Reset variables for later use
	preempt = 0;
	putInQueue = 0;
}

// Explicit context switch
void dispatch()
{
#ifndef BCC_BLOCK_IGNORE
	stack_lock;
#endif
	context_switch = 1;
	preempt = 1;
	unlock();
	TimerIntr();
#ifndef BCC_BLOCK_IGNORE
	stack_unlock;
#endif
}



