
#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include "init.h"
#include "pcb.h"
#include "thread.h"
#include "schedule.h"
#include "lst_slp.h"

// Global var(s)
unsigned oldTimerIntrOFF, oldTimerIntrSEG, retVal, tss, tsp;
extern volatile int is_timer_ready, allowContextSwitch;
extern int carg;
extern char** varg;

extern LST_SLEEP lst_sleeping;


// Define UserMainThread thread
class UserMainThread : public Thread
{
public:
	UserMainThread(unsigned int timeSlice) : Thread(4096, timeSlice){}
	virtual ~UserMainThread();
	virtual void run()
	{
		retVal = userMain(carg, varg);
	}
};

UserMainThread::~UserMainThread()
{
	waitToComplete();
}

// Wait until some PCB is ready to go
void sys_wait()
{
	while(1);
}

// Initialize IVT for timer
void init()
{
#ifndef BCC_BLOCK_IGNORE
	asm {
		cli
		push ax
		push es
		mov ax, 0
		mov es, ax

		mov ax, word ptr es:0020h
		mov word ptr oldTimerIntrOFF, ax
		mov ax, word ptr es:0022h
		mov word ptr oldTimerIntrSEG, ax

		// Set-up a new interrupt to the entry 08h
		mov word ptr es:0020h, offset TimerIntr
		mov word ptr es:0022h, seg TimerIntr

		// Move old interrupt to the entry 60h
		mov ax, oldTimerIntrOFF
		mov word ptr es:0180h, ax
		mov ax, oldTimerIntrSEG
		mov word ptr es:0182h, ax

		pop es
		pop ax
		sti
	}
#endif
}

// Restore initial values of the IVT
void restore()
{
#ifndef BCC_BLOCK_IGNORE
	asm {
		cli
		push ax
		push es
		mov ax, 0
		mov es, ax

		// Restore values to the entry 08h
		mov ax, word ptr es:0180h
		mov word ptr es:0020h, ax
		mov ax, word ptr es:0182h
		mov word ptr es:0022h, ax

		pop es
		pop ax
		sti
	}
#endif
}

// Do something function
void doSomething()
{
	lock();

	// Create main & wait threads
	PCB::main = new PCB(30);
	PCB::wait = new PCB(sys_wait);

	// Set timer to be ready
	is_timer_ready = 1;

	// Create userMain
	UserMainThread userMain(10);
	userMain.start();

	// Change context to userMain explicitly!
	dispatch();

	// Delete UserMain, main & wait threads
	userMain.~UserMainThread();
	delete PCB::main;
	delete PCB::wait;
}

void lock()
{
	allowContextSwitch = 0;
}

void unlock()
{
	allowContextSwitch = 1;
}

// System exit
void sys_exit(int code)
{
	restore();
	exit(code);
}

// Update sleeping threads
void update_sleeping_threads()
{
	lst_sleeping.update();
}

