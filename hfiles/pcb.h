#ifndef _pcb_h_
#define _pcb_h_

#include "lst_fifo.h"
class Thread;

class PCB
{
public:
	// Currently running & idle PCB
	static PCB* running;
	static PCB* main;
	static PCB* wait;

	PCB(Thread*, unsigned int, unsigned int);
	PCB(unsigned int timeSlice);
	PCB( void (*f)() );
	static void wrapper();
	~PCB();

	// Setters
	void setSS(unsigned int);
	void setSP(unsigned int);
	void setBP(unsigned int);
	void clearStack();
	void setFinished();
	void setSleep();
	void rstSleep();
	void setBlocked();
	void rstBlocked();
	void blockThread(PCB*);
	void unblockThreads();

	// Getters
	unsigned int getSS();
	unsigned int getSP();
	unsigned int getBP();
	unsigned int getTime();
	unsigned int isFinished();
	unsigned int isSleeping();
	unsigned int isBlocked();
	unsigned int isReady();


private:
	Thread* myThread;
	LIST_FIFO blocked;
	unsigned int ss, sp, bp;
	unsigned int* stack;
	unsigned int time;
	unsigned int is_finished;
	unsigned int is_sleeping;
	unsigned int is_blocked;
};

#endif
