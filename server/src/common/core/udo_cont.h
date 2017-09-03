#ifndef udo_cont_h__
#define udo_cont_h__

#define udo_cont(type,member,it) ((type*)(((char*)it)-&(((type*)0)->member)))

#endif // udo_cont_h__
