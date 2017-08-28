#include "udo_http_req.h"
#include <stdlib.h>

void udo_http_req_init(udo_http_req* self)
{
	memset(self->req_source, 0, 1024);
}

void udo_http_req_deserialize(udo_http_req* self, char* req, int req_len)
{
	char method[10] = "";
	sscanf_s(req, "%s[^ ]", method,sizeof(method));
	if (strcmp(method, "GET") == 0)
	{
		char* buf = &req[4];
		char* bu1 = self->req_source;
		while (*buf != ' ')
		{
			if (*buf == '/')
			{
				*bu1 = '\\';
			}
			else
			{
				*bu1 = *buf;
			}
			bu1++;
			buf++;
		}
	}
	if (strcmp(self->req_source, "\\") == 0)
	{
		char str[1024] = "\\web\\html\\HomePage.html";
		strcpy(self->req_source, str);
	}

}

char* udo_http_req_res(udo_http_req* self)
{
	return self->req_source;
}

void udo_http_req_copy(udo_http_req* self, udo_http_req* other)
{
	udo_http_req_init(self);
	memcpy_s(self->req_source,1024, other->req_source,1024);
}
