#include "udo_http_rep.h"
#include <stdio.h>
#include "../common/config/udo_config.h"


void udo_http_rep_init(udo_http_rep* self, udo_http_req* req)
{
	udo_http_req_copy(&self->req, req);
	self->rep = NULL;
}

int udo_http_rep_serialize(udo_http_rep* self)
{
	FILE* f = NULL;
	udo_config config;
	int length = 0;
	int curpos;
	char res[4096] = "";
	char header[1024] = "";
	sprintf_s(header, 1024, "HTTP/1.0 200 ok\r\n"
		"sssssss\r\n"
		"Content-Type: %s\r\n"
		"\r\n", self->req.content_type);
	udo_config_init(&config);
	fopen_s(&f, udo_config_get_res(&config, udo_http_req_res(&self->req)), "rb");
	if (!f)
	{
		return UDO_HTTP_REP_SERIALIZE_FAILED;
	}
	curpos = ftell(f);
	fseek(f, 0L, SEEK_END);
	length = ftell(f);
	fseek(f, curpos, SEEK_SET);
	fread_s(res, 4096, 4096, 1, f);
	self->rep_length = strlen(header) + length;
	if (self->rep)
	{
		free(self->rep);
	}
	self->rep = (char*)malloc(self->rep_length+1);
	memcpy(self->rep, header, strlen(header));
	char* tp = self->rep + strlen(header);
	memcpy(tp, res, length);
	*(tp + length) = '\0';
	
	fclose(f);
	return UDO_HTTP_REP_SERIALIZE_SUCCESSED;
}

char* udo_http_rep_deserialize(udo_http_rep* self)
{
	return self->rep;
}

int udo_http_rep_length(udo_http_rep* self)
{
	return self->rep_length;
}

void udo_http_rep_term(udo_http_rep* self)
{
	if (self->rep)
	{
		free(self->rep);
	}
}

