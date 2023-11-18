#ifndef _ivtentry_h_
#define _ivtentry_h_

#include "init.h"

class IVTEntry
{
public:
	static IVTEntry* interruptRoutines[256];

	IVTEntry(unsigned char ivtNo, farPointer routine);
	~IVTEntry();

	void signal();
	void callOldRoutine();

private:
	unsigned char no;
	farPointer newRoutine;
	friend class KernelEv;
};

#endif
