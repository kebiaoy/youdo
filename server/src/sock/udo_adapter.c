#include <pcap.h>
#include <iphlpapi.h>
#include "../common/error/udo_error.h"
#include "udo_adapter.h"



#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

static char* str_cap = "rpcap://\\Device\\NPF_";

static void udo_adapter_get(udo_adapter* self, int adapter_type);

static void udo_adapter_open(udo_adapter* self);

void udo_adapter_init(udo_adapter* self, int adapter_type)
{
	memset(self->ip, 0, UDO_IP_ADDR_FORMAT_LEN);
	memset(self->mac_addr, 0, UDO_IP_ADDR_FORMAT_LEN);
	memset(self->adapter_name, 0, UDO_ADAPTER_NAME_LENGTH);

	self->adapter_type = adapter_type;
	self->mac_len = 0;
	udo_adapter_get(self, adapter_type);
	udo_adapter_open(self);

}

unsigned char* udo_adapter_mac(udo_adapter* self)
{
	return self->mac_addr;
}

unsigned char* udo_adapter_ip(udo_adapter* self)
{
	return self->ip;
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
			memcpy(self->ip, pAdapter->IpAddressList.IpAddress.String,UDO_IP_ADDR_FORMAT_LEN);
			memcpy(self->mac_addr, pAdapter->Address, pAdapter->AddressLength);
			self->mac_len = pAdapter->AddressLength;
			memcpy(self->adapter_name, pAdapter->AdapterName, strlen(pAdapter->AdapterName));
			memcpy(self->gateway_ip, pAdapter->GatewayList.IpAddress.String, UDO_IP_ADDR_FORMAT_LEN);
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

unsigned char* udo_adapter_gateway(udo_adapter* self)
{
	return self->gateway_ip;
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

int udo_adapter_rpacket(udo_adapter* self, unsigned char** packet, int *packet_len)
{
	int ret = 0;
	struct pcap_pkthdr *header;
	ret = pcap_next_ex(self->adhandle, &header, packet);
	if (ret > 0)
	{
		*packet_len = header->len;
	}
	return ret;
}
