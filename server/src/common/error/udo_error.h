#ifndef UDO_ERROR_H__
#define UDO_ERROR_H__

#include <stdio.h>

void udo_system_error();

#define udo_assert(x)\
do\
{\
if (!(x))\
{\
	printf("����ʧ��:%s\n�ļ�:%s\n������:%d\n", #x, __FILE__, __LINE__); \
	abort(); \
}\
}while (0)

#define udo_assert_str(x,str)\
	if (!(x))\
	{\
	printf("error:%s  ", str); \
	udo_assert(x); \
	}
	

#define udo_assert_system(x)\
do\
{\
if (!(x))\
{\
	printf("����ʧ��:%s\n�ļ�:%s\n������:%d\n", #x, __FILE__, __LINE__); \
	udo_system_error();\
	abort(); \
}\
}while (0)

#endif