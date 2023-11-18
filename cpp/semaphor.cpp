
#include "lst_fifo.h"
#include "semaphor.h"
#include "krnlsem.h"

// Create new semaphore
Semaphore::Semaphore(int init)
{
	myImpl = new KernelSem(init);
}

// Signal operation on the semaphore
void Semaphore::signal()
{
	myImpl->signal();
}

// Wait on the semaphore
int Semaphore::wait(int toBlock)
{
	return myImpl->wait(toBlock);
}

// Get value
int Semaphore::val() const { return myImpl->val(); }

// Destroy semaphore
Semaphore::~Semaphore() { delete myImpl; }
