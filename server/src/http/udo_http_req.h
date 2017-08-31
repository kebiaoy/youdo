#ifndef UDO_HTTP_REQ_H__
#define UDO_HTTP_REQ_H__

#include <windows.h>
#define UDO_HTTP_CONTENT_TYPE_LENGTH 128
typedef struct udo_http_req
{
	char req_source[MAX_PATH*2];
	char content_type[UDO_HTTP_CONTENT_TYPE_LENGTH];
}udo_http_req;

void udo_http_req_init(udo_http_req* self);

void udo_http_req_deserialize(udo_http_req* self, char* req, int req_len);

char* udo_http_req_res(udo_http_req* self);

void udo_http_req_copy(udo_http_req* self, udo_http_req* other);

void udo_http_req_term(udo_http_req* self);
#endif