
#include <stdio.h>
#include "init.h"
#include "pcb.h"
#include "lst_slp.h"
#include "schedule.h"

LST_SLEEP::LST_SLEEP() : head(0) {}
extern unsigned int putInQueue;

// Destroy list and it's elements
LST_SLEEP::~LST_SLEEP()
{
	ElemLstSleep* tek = head;
	while( tek != 0 )
	{
		ElemLstSleep *old = tek;
		tek = tek->next;
		delete old;
	};
}

// Put element into the list
void LST_SLEEP::put( PCB* pcb, unsigned int time )
{
	#ifndef BCC_BLOCK_IGNORE
	stack_lock;
	#endif
	unsigned int sub = 0;
	// We are putting in empty list
	if ( head == 0 )
	{
		head = new ElemLstSleep(pcb, time);
	} else // We are putting in non-empty list
	{
		ElemLstSleep *q = 0, *tek = head, *novi;
		while( tek != 0 && time >= tek->time )
		{
			time -= tek->time;
			q = tek;
			tek = tek->next;
		};

		// Create new element
		novi = new ElemLstSleep( pcb, time );

		// time < first->time
		if ( q == 0 )
		{
			novi->next = head;
			head->time -= time;
			head = novi;
		} else
		{
			q->next = novi;
			novi->next = tek;
			if ( tek != 0 ) tek->time -= time;
		};
	};
	#ifndef BCC_BLOCK_IGNORE
	stack_unlock;
	#endif
}

void LST_SLEEP::update()
{
	if ( head == 0 ) return;
	head->time--;
	while( head && head->time==0 )
	{
		ElemLstSleep *temp = head;
		head->pcb->rstSleep();
		Scheduler::put( head->pcb );
		putInQueue = 1;
		head = head->next;
		delete temp;
	};
}

void LST_SLEEP::pisi()
{
	ElemLstSleep *tek = head;
	while( tek != 0 )
	{
		printf("%d ", tek->time);
		tek = tek->next;
	}
	printf("\n");
}
