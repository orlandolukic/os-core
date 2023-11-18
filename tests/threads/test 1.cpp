
/*
 * Test for threads
 * ==================
 *

// Define UserMainThread thread
class MyThread : public Thread
{
public:
	MyThread(unsigned int timeSlice, unsigned int tID) : Thread(4096, timeSlice), threadID(tID) {}
	virtual ~MyThread() { waitToComplete(); }
	virtual void run()
	{
		for (int i=0; i<7; i++)
		{
			if ( i == 3 ) { printf("SLEEPING ... \n"); Thread::sleep(20); }
			if ( i == 5 ) { printf("SLEEPING SECOND TIME ... \n"); Thread::sleep(60); }
			printf("%d. MyThread (%d)\n", i, threadID);
			for (unsigned int h=0; h<20000; h++)
				for (unsigned int k=0; k<20000; k++);
		};
	}
private:
	unsigned int threadID;
};

================================================================================================================

	MyThread* arr[20];
	for (int i=0; i<20; i++)
	{
		arr[i] = new MyThread(2, i+1);
		arr[i]->start();
	};

	for (int j=0; j<10; j++)
	{
		if ( j == 5 )
		{
			printf("SLEEPING MAIN ... \n");
			Thread::sleep(40);
		};
		printf("%d. UserMain\n", j);
		for (unsigned int h=0; h<20000; h++)
			for (unsigned int k=0; k<20000; k++);
	};

	for (int g=0; g<20; g++) delete arr[g];


	//delete t;
	delete[] arr;
	return 1;

*/
