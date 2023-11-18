
#ifndef _kernelsem_h_
#define _kernelsem_h_

// List of all blocked PCB's
class LIST_FIFO;

// Class of all semaphores in kernel
class KernelSem
{
public:
	KernelSem(int init);
	~KernelSem();

	int wait(int toBlock);
	void signal();

	int val() const;
private:
	int value;
	LIST_FIFO blocked;
};

#endif
