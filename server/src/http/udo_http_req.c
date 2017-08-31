#include <stdlib.h>
#include "../common/string/udo_rex.h"
#include "udo_http_req.h"


void udo_http_req_init(udo_http_req* self)
{
	memset(self->content_type, 0, UDO_HTTP_CONTENT_TYPE_LENGTH);
	memset(self->req_source, 0, MAX_PATH*2);
}

void udo_http_req_deserialize(udo_http_req* self, char* req, int req_len)
{
	char method[10] = "";
	udo_rex rex;
	udo_rex_init(&rex);
	if (udo_rex_match(&rex, req, req_len, "^\\b\\w+\\b") == UDO_REX_NO_MATCH)
	{
		return;
	}
	int match_length;
	char* match_str = udo_rex_index(&rex, 0, &match_length);

	if (strncmp(match_str, "GET", match_length) == 0)
	{
		if (udo_rex_match(&rex, match_str + match_length, req_len - match_length, "\\s+([^\\s]+)") == UDO_REX_NO_MATCH)
		{
			return;
		}
		match_str = udo_rex_index(&rex, 1, &match_length);

		if (match_length > MAX_PATH)
		{
			return;
		}
		for (int i = 0; i < match_length; ++i)
		{
			if (match_str[i] == '/')
			{
				self->req_source[i] = '\\';
				continue;
			}
			self->req_source[i] = match_str[i];
		}
	}
	if (strcmp(self->req_source, "\\") == 0)
	{
		char* str = "\\web\\html\\HomePage.html";
		memcpy(self->req_source, str,strlen(str));
	}
	if (udo_rex_match(&rex, self->req_source, 
		strlen(self->req_source), "[^\\.]+\\.(\\w+)$") == UDO_REX_NO_MATCH)
	{
		return;
	}
	match_str = udo_rex_index(&rex, 1, &match_length);
	if (strncmp(match_str, "png", match_length) == 0)
	{
		char* str = "image/png";
		memcpy(self->content_type, str, strlen(str));
	}
	else if (strncmp(match_str, "ico", match_length) == 0)
	{
		char* str = "image/png";
		memcpy(self->content_type, str, strlen(str));
	}
	else if (strncmp(match_str, "html", match_length) == 0)
	{
		char* str = "text/html";
		memcpy(self->content_type, str, strlen(str));
	}
	udo_rex_term(&rex);
}

char* udo_http_req_res(udo_http_req* self)
{
	return self->req_source;
}

void udo_http_req_copy(udo_http_req* self, udo_http_req* other)
{
	udo_http_req_init(self);
	memcpy_s(self->req_source, MAX_PATH * 2, other->req_source, MAX_PATH * 2);
	memcpy_s(self->content_type, UDO_HTTP_CONTENT_TYPE_LENGTH, 
		other->content_type, UDO_HTTP_CONTENT_TYPE_LENGTH);
}

void udo_http_req_term(udo_http_req* self)
{

}
