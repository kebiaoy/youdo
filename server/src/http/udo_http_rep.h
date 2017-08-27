#ifndef UDO_HTTP_REP_H__
#define UDO_HTTP_REP_H__

#include "udo_http_req.h"

typedef struct udo_http_rep
{
	char rep[4096];
	udo_http_req req;
}udo_http_rep;

void udo_http_rep_init(udo_http_rep* self, udo_http_req* req);

char* udo_http_rep_serialize(udo_http_rep* self);

#endif // !UDO_HTTP_REP_H__
