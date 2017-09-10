#include "../common/error/udo_error.h"
#include "../common/core/udo_cont.h"
#include "udo_link_layer.h"
#include "udo_adapter.h"
#include "udo_server_dns.h"
#include "udo_arp.h"
#include "udo_host.h"
#include "udo_arp_spoofing.h"

static void udo_server_dns_run(void* arg);

void udo_server_dns_init(udo_server_dns* self, char* dns_name)
{
	int dns_len = strlen(dns_name);
	udo_assert(UDO_SERVER_DNS_LEN > dns_len);
	memset(self->dns_name, 0, UDO_SERVER_DNS_LEN);
	memcpy(self->dns_name, dns_name, dns_len);
	udo_thread_init(&self->thread, udo_server_dns_run, self);
}

void udo_server_dns_run(void* arg)
{
	//关于网络访问层 应该放到适配器类中操作
	udo_server_dns* self = (udo_server_dns*)arg; 
	udo_adapter adapter;
	char adapter_name[256] = "";
	int res = 0;
	int packet_len;
	unsigned char* u_packet;
	unsigned counter = 0;

	udo_adapter_init(&adapter, UDO_IF_TYPE_ETHERNET);

	while ((res = udo_adapter_rpacket(&adapter, &u_packet, &packet_len)) >= 0)
	{
		if (res == 0)
		{
			continue;
		}
	}


	udo_adapter_term(&adapter);
}

void udo_server_dns_start(udo_server_dns* self)
{
	udo_thread_start(&self->thread);
}

void udo_server_term(udo_server_dns* self)
{
	udo_thread_term(&self->thread);
}

