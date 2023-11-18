
#include "kernelEv.h"
#include "event.h"

// Create KernelEvent
Event::Event(IVTNo ivtNo)
{
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	myImpl = new KernelEv(ivtNo, this);
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
}

// Destroy KernelEvent
Event::~Event()
{
#ifndef BCC_BLOCK_IGNORE
	stack_lock;
#endif
	delete myImpl;
#ifndef BCC_BLOCK_IGNORE
	stack_unlock;
#endif
}

// WAIT: Talk to the KernelEvent
void Event::wait()
{
	myImpl->wait();
}

// SIGNAL: Talk to the KernelEvent
void Event::signal()
{
	myImpl->signal();
}


