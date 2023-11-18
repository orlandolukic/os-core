
/*

Semaphore* s = new Semaphore(0);
int global = 0;

class Producer : public Thread
{
public:
	Producer(unsigned int timeSlice, unsigned int tID) : Thread(4096, timeSlice), threadID(tID) {}
	virtual ~Producer() { waitToComplete(); }
	virtual void run()
	{
		printf("SETTING VAR ...\n");
		global = 20;
		s->signal();
	}
private:
	unsigned int threadID;
};

class Consumer : public Thread
{
public:
	Consumer(unsigned int timeSlice, unsigned int tID) : Thread(4096, timeSlice), threadID(tID) {}
	virtual ~Consumer() { waitToComplete(); }
	virtual void run()
	{
		printf("WAINTING ...\n");
		if ( s->wait(1) ) printf("THREAD HAS BEEN BLOCKED!\n");
		printf("SLEEPING PRINTF ... \n");
		Thread::sleep(40);
		printf("GLOBAL VAR global=%d\n", global);
	}
private:
	unsigned int threadID;
};






 */
