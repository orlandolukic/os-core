
#include <stdio.h>
#include "thread.h"

class MyThread : public Thread
{
public:
	MyThread() : Thread(1024) {}

	virtual void run()
	{
		for (int i=0; i<100; i++)
			printf("U MyThread i=%d\n", i);
	}
};




