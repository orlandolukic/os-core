#ifndef _event_h_
#define _event_h_

typedef unsigned char IVTNo;
class KernelEv;
#include "ivtentry.h"

class Event
{
public:
	Event(IVTNo ivtNo);
	~Event();

	void wait();

protected:
	friend class KernelEv;
	void signal(); // can call KernelEv

private:
	KernelEv* myImpl;
};

// PREPAREENTRY Macro
#define PREPAREENTRY(_numEntry, _callOld); \
		void interrupt inter##_numEntry( ... ) { \
			if ( IVTEntry::interruptRoutines[(unsigned char)_numEntry] ) { \
					(IVTEntry::interruptRoutines[(unsigned char)_numEntry])->signal(); \
					if(_callOld == 1) (IVTEntry::interruptRoutines[ (unsigned char) _numEntry ])->callOldRoutine(); \
			}; \
		}; \
		IVTEntry newIVTEntry##_numEntry((unsigned char)_numEntry, inter##_numEntry);

#endif
