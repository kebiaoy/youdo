#ifndef udo_server_dns_h__
#define udo_server_dns_h__
#include "../common/core/udo_thread.h"

#define UDO_SERVER_DNS_LEN 32

typedef struct udo_server_dns
{
	char dns_name[UDO_SERVER_DNS_LEN];
	udo_thread thread;
}udo_server_dns;

void udo_server_dns_init(udo_server_dns* self, char* dns_name);

void udo_server_dns_start(udo_server_dns* self);

void udo_server_term(udo_server_dns* self);

#endif // udo_server_dns_h__
