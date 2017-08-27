#include "udo_error.h"
#include <windows.h>


void udo_system_error()
{
	int nErrCode = GetLastError();
	char msg[1024] = "";

	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, nErrCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)msg, 1024, NULL);
	printf("%s", msg);
}