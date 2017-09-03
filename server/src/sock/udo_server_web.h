#ifndef UDO_SERVER_WEB_H__
#define UDO_SERVER_WEB_H__

#define UDO_SERVER_DNS_LEN 32

typedef struct udo_server_web
{
	char domain_name[UDO_SERVER_DNS_LEN];
}udo_server_web;


void udo_server_web_init(udo_server_web* self, char* domain_name);

void udo_server_web_loop(udo_server_web* self);

void udo_server_web_term(udo_server_web* self);

#endif // !UDO_SERVER_WEB_H__
