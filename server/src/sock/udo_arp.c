#include <WinSock2.h>
#include "../common/error/udo_error.h"
#include "../common/string/udo_num.h"
#include "udo_adapter.h"
#include "udo_arp.h"



void udo_arp_init(udo_arp* self,struct udo_adapter* adapter)
{
	self->adapter = adapter;
	self->op = 0;
	self->procotol_type = UDO_ETHERNET_IP;
	self->hardware_type = 1;
	self->hardware_len = UDO_MAC_ADDR_LEN;
	self->procotol_len = UDO_IP_ADDR_LEN;
	memset(self->src_mac_addr, 0, UDO_MAC_ADDR_LEN);
	memset(self->src_ip_addr, 0, UDO_IP_ADDR_LEN);
	memset(self->dst_mac_addr, 0, UDO_MAC_ADDR_LEN);
	memset(self->dst_ip_addr, 0, UDO_IP_ADDR_LEN);
}

void udo_arp_setop(udo_arp* self, int op)
{
	self->op = op;
}

int udo_arp_getop(udo_arp* self)
{
	return self->op;
}

void udo_arp_setsma(udo_arp* self, unsigned char* sma)
{
	
	udo_assert(sma); 
	int j = 0;
	for (int i = 0; i < UDO_MAC_ADDR_FORMAT_LEN; ++i)
	{
		if (sma[i] == UDO_MAC_ADDR_FORMAT_CHAR)
		{
			j++;
			continue;
		}
		self->src_mac_addr[j] = self->src_mac_addr[j] * 16 + udo_number_char(sma[i]);
	}
}

char* udo_arp_getsma(udo_arp* self)
{
	return self->src_mac_addr;
}

void udo_arp_setsia(udo_arp* self, unsigned char* sia)
{
	udo_assert(sia);
	unsigned int ip = inet_addr(sia);
	memcpy(self->src_ip_addr, (void*)&ip, UDO_IP_ADDR_LEN);
}

char* udo_arp_getsia(udo_arp* self)
{
	return self->src_ip_addr;
}

void udo_arp_setdma(udo_arp* self, unsigned char* dma)
{
	udo_assert(dma);
	int j = 0;
	for (int i = 0; i < UDO_MAC_ADDR_FORMAT_LEN; ++i)
	{
		if (dma[i] == UDO_MAC_ADDR_FORMAT_CHAR)
		{
			j++;
			continue;
		}
		self->dst_mac_addr[j] = self->dst_mac_addr[j] * 16 + udo_number_char(dma[i]);
	}
}

char* udo_arp_getdma(udo_arp* self)
{
	return self->dst_mac_addr;
}

void udo_arp_setdia(udo_arp* self, unsigned char* dia)
{
	udo_assert(dia);
	unsigned int ip = inet_addr(dia);
	memcpy(self->dst_ip_addr, (void*)&ip, UDO_IP_ADDR_LEN);
}

char* udo_arp_getdia(udo_arp* self)
{
	return self->dst_ip_addr;
}

void udo_arp_deserialize(udo_arp* self, unsigned char* packet, int start_offset)
{
	udo_assert(packet);
	unsigned char* t_packet = &packet[start_offset];
	self->hardware_type = htons(*((unsigned short*)t_packet));
	t_packet += sizeof(self->hardware_type);
	self->procotol_type = htons(*((unsigned short*)t_packet));
	t_packet += sizeof(self->procotol_type);
	self->hardware_len = t_packet[0];
	t_packet += sizeof(self->hardware_len);
	self->procotol_len = t_packet[0];
	t_packet += sizeof(self->procotol_len);
	self->op = htons(*((unsigned short*)t_packet));
	t_packet += sizeof(self->op);
	memcpy(self->src_mac_addr, t_packet, UDO_MAC_ADDR_LEN);
	t_packet += UDO_MAC_ADDR_LEN;
	memcpy(self->src_ip_addr, t_packet, UDO_IP_ADDR_LEN);
	t_packet += UDO_IP_ADDR_LEN;
	memcpy(self->dst_mac_addr, t_packet, UDO_MAC_ADDR_LEN);
	t_packet += UDO_MAC_ADDR_LEN;
	memcpy(self->dst_ip_addr, t_packet, UDO_IP_ADDR_LEN);
	t_packet += UDO_IP_ADDR_LEN;
}

void udo_arp_serialize(udo_arp* self, unsigned char* packet, int start_offset)
{
	udo_assert(packet);
	unsigned char* t_packet = &packet[start_offset];
	unsigned short* short_packet=(unsigned short*)t_packet;
	short_packet[0] = htons(self->hardware_type);
	t_packet += sizeof(self->hardware_type);
	short_packet = (unsigned short*)t_packet;
	short_packet[0] = htons(self->procotol_type);
	t_packet += sizeof(self->procotol_type);
	t_packet[0] = self->hardware_len;
	t_packet += sizeof(self->hardware_len);
	t_packet[0] = self->procotol_len;
	t_packet += sizeof(self->procotol_len);
	short_packet = (unsigned short*)t_packet;
	short_packet[0] = htons(self->op);
	t_packet += sizeof(self->op);
	memcpy(t_packet, self->src_mac_addr, UDO_MAC_ADDR_LEN);
	t_packet += UDO_MAC_ADDR_LEN;
	memcpy(t_packet, self->src_ip_addr, UDO_IP_ADDR_LEN);
	t_packet += UDO_IP_ADDR_LEN;
	memcpy(t_packet, self->dst_mac_addr, UDO_MAC_ADDR_LEN);
	t_packet += UDO_MAC_ADDR_LEN;
	memcpy(t_packet, self->dst_ip_addr, UDO_IP_ADDR_LEN);
	t_packet += UDO_IP_ADDR_LEN;
}

void udo_arp_term(udo_arp* self)
{

}

