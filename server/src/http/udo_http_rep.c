#include "udo_http_rep.h"
#include <stdio.h>
#include "../common/config/udo_config.h"


void udo_http_rep_init(udo_http_rep* self, udo_http_req* req)
{
	udo_http_req_copy(&self->req, req);
	memset(self->rep, 0, 4096);
}

char* udo_http_rep_serialize(udo_http_rep* self)
{
	FILE* f = NULL;
	udo_config config;
	int length = 0;
	char res[4096] = "";
	char header[1024] = "HTTP/1.0 200 ok\r\n"
		"sssssss\r\n"
		"Content-Type: text/html\r\n"
		"\r\n";
	memcpy(self->rep, header,strlen(header));
	udo_config_init(&config);
	fopen_s(&f, udo_config_get_res(&config, udo_http_req_res(&self->req)), "r+b");
	if (!f)
	{
		return "ss";
	}
	fread_s(res, 4096, 4096, 1, f);
	length = strlen(self->rep);
	strcat(self->rep, res);
	if (self->rep[length - 1] != '\n'&&self->rep[length - 2] != '\r')
	{
		strcat(self->rep, "\r\n");
	}
	fclose(f);
	return self->rep;
}

