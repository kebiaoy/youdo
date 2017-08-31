#ifndef UDO_HTTP_REP_H__
#define UDO_HTTP_REP_H__

#include "udo_http_req.h"

#define UDO_HTTP_REP_SERIALIZE_FAILED 0
#define UDO_HTTP_REP_SERIALIZE_SUCCESSED 1

typedef struct udo_http_rep
{
	char* rep;
	int rep_length;
	udo_http_req req;
}udo_http_rep;

void udo_http_rep_init(udo_http_rep* self, udo_http_req* req);

int udo_http_rep_serialize(udo_http_rep* self);

int udo_http_rep_length(udo_http_rep* self);

char* udo_http_rep_deserialize(udo_http_rep* self);

void udo_http_rep_term(udo_http_rep* self);
#endif // !UDO_HTTP_REP_H__
