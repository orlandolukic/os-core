
#include <stdio.h>
#include "pcb.h"
#include "schedule.h"
#include "lst_fifo.h"

// Ctor
LIST_FIFO::LIST_FIFO() : first(0), last(0), elems(0) {};

// Get first element of the list
PCB* LIST_FIFO::getFirst()
{
	if ( first == 0 ) return 0;
	Elem* ret = first;
	first = first->next;
	if (first == 0) last = 0;
	elems--;
	return ret->content;
}

// Destroy all the listed data
LIST_FIFO::~LIST_FIFO()
{
	Elem* tek = first;
	while( tek != 0 )
	{
		Elem* p = tek;
		tek = tek->next;
		delete p;
	};
	first = last = 0;
}

// Put content into the list
void LIST_FIFO::put(PCB* p)
{
	if ( first == 0 ) first = last = new Elem(p); else last = last->next = new Elem(p);
	elems++;
}

// Put element to the list with time
void LIST_FIFO::put(PCB* p, unsigned int time)
{
	if ( first == 0 ) first = last = new Elem(p, time); else last = last->next = new Elem(p, time);
	elems++;
}

// Obtain number of elements inside the list
unsigned int LIST_FIFO::elements() const { return elems; }

// Go through list and update time
void LIST_FIFO::updateTime()
{
	Elem *tek = first, *q = 0;
	while(tek != 0)
	{
		if ( --tek->timeSlice == 0 )
		{
			// Put thread into the scheduler
			Scheduler::put( tek->content );

			// Reset sleeping state
			tek->content->rstSleep();

			// Remove element from the list
			Elem *t = tek;
			if ( q == 0 ) // We are removing first element
			{
				q     = tek;
				tek   = tek->next;
				first = tek;
				if ( first == 0 ) last = 0;
				delete t;
			} else // We are removing element inside the list
			{
				q->next = tek->next;
				tek = tek->next;
				if ( tek == 0 ) last = q;
				delete t;
			};
		} else
		{
			q   = tek;
			tek = tek->next;
		}
	}
}

void LIST_FIFO::putAllInScheduler()
{
	while( first )
	{
		PCB* t = getFirst();
		t->rstBlocked();
		Scheduler::put(t);
	}
}
