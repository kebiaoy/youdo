#include <windows.h>  
#include <stdio.h>  
#include <process.h>  
#include "udo_thread.h"

static unsigned __stdcall udo_thread_run(void* pArguments)
{
	udo_thread* self = (udo_thread*)pArguments;
	self->func(self->arg);
	_endthreadex(0);
	return 0;
}

void udo_thread_init(udo_thread* self, udo_thread_func func, void* arg)
{
	self->arg = arg;
	self->func = func;
}
void udo_thread_start(udo_thread* self)
{
	self->thread_handle = (HANDLE)_beginthreadex(NULL, 0, udo_thread_run, self, 0, &self->thread_id);
}

void udo_thread_term(udo_thread* self)
{
	WaitForSingleObject(self->thread_handle, INFINITE);
	CloseHandle(self->thread_handle);
}







