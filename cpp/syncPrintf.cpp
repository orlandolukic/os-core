#include <DOS.H>
#include <STDIO.H>
#include <STDARG.H>

void lock();
void unlock();

int syncPrintf(const char *format, ...)
{
	int res;
	va_list args;
	lock();
	va_start(args, format);
	res = vprintf(format, args);
	unlock();
	return res;
}
