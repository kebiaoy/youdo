#include "udo_http_rep.h"
#include <stdio.h>
#include "../common/config/udo_config.h"


void udo_http_rep_init(udo_http_rep* self, udo_http_req* req)
{
	udo_http_req_copy(&self->req, req);
	memset(self->rep, 0, 4096);
}

int udo_http_rep_serialize(udo_http_rep* self)
{
	FILE* f = NULL;
	udo_config config;
	int length = 0;
	int curpos;
	char res[4096] = "";
	char header[1024] = "HTTP/1.0 200 ok\r\n"
		"sssssss\r\n"
		"Content-Type: %s\r\n"
		"\r\n";
	sprintf_s(self->rep, 4096, header, self->req.content_type);
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
	self->rep_length = strlen(self->rep) + length;
	char* tp = self->rep + strlen(self->rep);
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

}

