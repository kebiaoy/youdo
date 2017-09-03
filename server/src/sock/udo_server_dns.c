#include <pcap.h>
#include "../common/error/udo_error.h"
#include "udo_link_layer.h"
#include "udo_adapter.h"
#include "udo_server_dns.h"
#include "udo_arp.h"

static char* str_cap = "rpcap://\\Device\\NPF_";

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
	udo_server_dns* self = (udo_server_dns*)arg; 
	udo_adapter adapter;
	char adapter_name[256] = "";
	char errbuf[PCAP_ERRBUF_SIZE]="";
	int cap_len = strlen(str_cap);
	pcap_t *adhandle;
	struct pcap_pkthdr *header;
	const u_char *pkt_data;
	int res = 0;

	udo_adapter_init(&adapter, MIB_IF_TYPE_ETHERNET);
	memcpy(adapter_name, str_cap, cap_len);
	memcpy(&adapter_name[cap_len], udo_adapter_name(&adapter),
		strlen(udo_adapter_name(&adapter)));
	adhandle = pcap_open(adapter_name, 65536, PCAP_OPENFLAG_PROMISCUOUS, 1000, NULL, errbuf);
	udo_assert_str(adhandle, errbuf);
	
	udo_arp arp_spool;
	udo_arp_init(&arp_spool);
	udo_arp_setop(&arp_spool, UDO_ARP_RESPONSE);

	udo_arp_setsma(&arp_spool, "f0:76:1c:13:ac:ed");
	udo_arp_setsia(&arp_spool, "192.168.1.1");
	udo_arp_setdma(&arp_spool, "08:23:b2:74:7c:a4");
	udo_arp_setdia(&arp_spool, "192.168.1.4");

	udo_link_layer link_layer_spool;
	udo_link_layer_init(&link_layer_spool);
	udo_link_layer_setdst(&link_layer_spool, "08:23:b2:74:7c:a4");
	udo_link_layer_setsrc(&link_layer_spool, "f0:76:1c:13:ac:ed");

	/*char []*/


	while ((res = pcap_next_ex(adhandle, &header, &pkt_data)) >= 0)
	{
		if (res == 0)
		{
			continue;
		}
		udo_link_layer link_layer;
		udo_link_layer_init(&link_layer);
		udo_link_layer_deserialize(&link_layer, pkt_data);
		if (strncmp(udo_adapter_mac(&adapter), udo_link_layer_getsrc(&link_layer),UDO_MAC_ADDR_LEN) != 0 )
		{
			if (udo_link_layer_gettype(&link_layer) != UDO_LINK_TYPE_ARP)
			{
				continue;
			}
			udo_arp arp;
			udo_arp_init(&arp);
			udo_arp_deserialize(&arp, pkt_data, UDO_LINK_LAYER_LEN);
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

