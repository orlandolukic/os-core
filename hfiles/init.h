#ifndef _init_h_
#define _init_h_

void setContextSwitch();

void lock();
void unlock();
void interrupt TimerIntr();
void tick();
void retFunction();
void sys_exit(int);
void sys_wait();
void update_sleeping_threads();
void dispatch();

int userMain(int, char*[]);
void init();
void restore();
void doSomething();

typedef void interrupt (*farPointer)(...);

#define stack_lock asm { \
	pushf; \
	cli; \
}

#define stack_unlock asm popf


#endif
