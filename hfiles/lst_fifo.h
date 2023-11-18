
#ifndef _lst_fifo_h_
#define _lst_fifo_h_

// Declarations & Definitions
class PCB;

// Structure for a list
struct Elem
{
	unsigned int timeSlice;
	PCB* content;
	Elem* next;
	Elem(PCB* content) : content(content), next(0), timeSlice(0) {}
	Elem(PCB* content, unsigned int time) : content(content), next(0), timeSlice(time) {}
};

// List containing PCB's
class LIST_FIFO
{
public:
	// Constructor
	LIST_FIFO();
	~LIST_FIFO();

	// Get | Put element into the list
	PCB* getFirst();
	void put(PCB*);
	void put(PCB*, unsigned int);
	void putAllInScheduler();

	// Go through the list and put elements into the Scheduler if needed
	void updateTime();

	// Get number of the elements
	unsigned int elements() const;

private:
	unsigned elems;
	Elem *first, *last;
};



#endif
