#include <WinSock2.h>
#include "../common/error/udo_error.h"
#include "udo_adapter.h"
#include "udo_server_dns.h"
#include "udo_arp_spoofing.h"

void udo_arp_spoofing_init(udo_arp_spoofing* self,
struct udo_server_dns* dns_server, struct udo_adapter* adapter)
{
	self->dns_server = dns_server;
	self->adapter = adapter;
	self->is_run = 0;
}

void udo_arp_spoofing_start(udo_arp_spoofing* self)
{
	unsigned int host_ip = inet_addr(udo_adapter_ip(self->adapter));
	unsigned int mask = inet_addr(udo_adapter_mask(self->adapter));
	host_ip = htonl(host_ip&mask);
	unsigned int count = htonl(~mask);

	struct in_addr addr;
	for (int i = 1; i < count; ++i)
	{
		addr.S_un.S_addr=htonl(++host_ip);
		char* ip = inet_ntoa(addr);
		printf("ip:%s\n", inet_ntoa(addr));
	}
	self->is_run = 1;
}
void udo_arp_spoofing_stop(udo_arp_spoofing* self)
{
	self->is_run = 0;
}

void udo_arp_spoofing_term(udo_arp_spoofing* self)
{
	udo_assert(!self->is_run);
}




