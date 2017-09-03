#ifndef udo_thread_h__
#define udo_thread_h__
#include <windows.h>

typedef void(*udo_thread_func)(void* arg);

typedef struct udo_thread
{
	void* arg;
	udo_thread_func func;
	HANDLE thread_handle;
	unsigned thread_id;
}udo_thread;


void udo_thread_init(udo_thread* self, udo_thread_func func,void* arg);

void udo_thread_start(udo_thread* self);

void udo_thread_term(udo_thread* self);


#endif // udo_thread_h__
