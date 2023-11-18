
#ifndef _LST_SLEEP_H_
#define _LST_SLEEP_H_

class PCB;
struct ElemLstSleep
{
	PCB* pcb;
	unsigned int time;
	ElemLstSleep *next;
	ElemLstSleep( PCB* p, unsigned int time ) { pcb = p; this->time = time; this->next = 0; }
	~ElemLstSleep() {}
};

class LST_SLEEP
{
private:
	ElemLstSleep *head;

public:
	LST_SLEEP();
	~LST_SLEEP();

	void put( PCB*, unsigned int );
	void update();
	void pisi();
};

#endif
