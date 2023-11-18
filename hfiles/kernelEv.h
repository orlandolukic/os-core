#ifndef _kernelEv_h_
#define _kernelEv_h_

#include "init.h"
class IVTEntry;
class PCB;
class Event;

class KernelEv
{
public:
	static KernelEv* oldInterruptRoutines[256];

	KernelEv(unsigned char ivtNo, Event* event);
	~KernelEv();

	void wait();	// Waiting for the interrupt to happen
	void signal();	// Signaling to the EventObject

private:
	Event* ev;
	int value;
	PCB* creator;
	unsigned char ivtNo;

protected:
	friend class IVTEntry;
	farPointer oldRoutine;
};

#endif
