
#include <stdio.h>
#include "event.h"
#include "ivtentry.h"
#include "semaphor.h"
#include "thread.h"
#include "init.h"
#include <iostream.h>
#include <stdlib.h>

//int syncPrintf(const char* format, ...);

const int n = 2;

//void tick(){}

class TestThread : public Thread
{
private:
	static int ID;
	int id;
public:

	TestThread(): Thread(defaultStackSize,2), id(++ID) {};
	~TestThread()
	{
		waitToComplete();
	}
	int getId() { return id; }
protected:

	void run();

};
int TestThread::ID = 0;

void TestThread::run()
{

	int buffer=2;

	for(int i=0;i<32000;i++)
	{
		buffer = 4096/2048;
		for (int j = 0; j < 1024; j++)
		{
			buffer = buffer*2;
			if(buffer%2)
				buffer = 2;
		}
	}

}


