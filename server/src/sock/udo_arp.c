#include <WinSock2.h>
#include "../common/error/udo_error.h"
#include "../common/string/udo_num.h"
#include "udo_link_layer.h"
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
	udo_ip_init(&self->src_ip_addr);
	memset(self->dst_mac_addr, 0, UDO_MAC_ADDR_LEN);
	udo_ip_init(&self->dst_ip_addr);
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
	memcpy(self->src_mac_addr, sma, UDO_MAC_ADDR_LEN);
}

char* udo_arp_getsma(udo_arp* self)
{
	return self->src_mac_addr;
}

void udo_arp_setsia(udo_arp* self, udo_ip* sia)
{
	udo_assert(sia);
	udo_ip_copy(&self->src_ip_addr, sia);
}

udo_ip* udo_arp_getsia(udo_arp* self)
{
	return &self->src_ip_addr;
}

void udo_arp_setdma(udo_arp* self, unsigned char* dma)
{
	udo_assert(dma);
	memcpy(self->dst_mac_addr, dma, UDO_MAC_ADDR_LEN);
}

char* udo_arp_getdma(udo_arp* self)
{
	return self->dst_mac_addr;
}

void udo_arp_setdia(udo_arp* self, udo_ip* dia)
{
	udo_assert(dia);
	udo_ip_copy(&self->dst_ip_addr, dia);
}

udo_ip* udo_arp_getdia(udo_arp* self)
{
	return &self->dst_ip_addr;
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
	udo_ip_setnet(&self->src_ip_addr, t_packet);
	t_packet += UDO_IP_ADDR_LEN;
	memcpy(self->dst_mac_addr, t_packet, UDO_MAC_ADDR_LEN);
	t_packet += UDO_MAC_ADDR_LEN;
	udo_ip_setnet(&self->dst_ip_addr, t_packet);
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
	memcpy(t_packet, udo_ip_getnet(&self->src_ip_addr), UDO_IP_ADDR_LEN);
	t_packet += UDO_IP_ADDR_LEN;
	memcpy(t_packet, self->dst_mac_addr, UDO_MAC_ADDR_LEN);
	t_packet += UDO_MAC_ADDR_LEN;
	memcpy(t_packet, udo_ip_getnet(&self->dst_ip_addr), UDO_IP_ADDR_LEN);
	t_packet += UDO_IP_ADDR_LEN;
}

void udo_arp_term(udo_arp* self)
{
	udo_ip_term(&self->src_ip_addr);
	udo_ip_term(&self->dst_ip_addr);
}

void udo_arp_resonse(udo_arp* self)
{
	unsigned char packet[UDO_ARP_TOTAL_LEN] = "";
	udo_link_layer link_layer_spool;
	udo_link_layer_init(&link_layer_spool);
	udo_link_layer_setdst(&link_layer_spool, self->dst_mac_addr);
	udo_link_layer_setsrc(&link_layer_spool, self->src_mac_addr);
	udo_link_layer_settype(&link_layer_spool, UDO_LINK_TYPE_ARP);
	udo_link_layer_serialize(&link_layer_spool, packet, 0);
	udo_arp_serialize(self, packet, UDO_LINK_LAYER_LEN);
	udo_adapter_send(self->adapter, packet, UDO_ARP_TOTAL_LEN);
}

void udo_arp_request(udo_arp* self, udo_ip* ip)
{
	unsigned char packet[UDO_ARP_TOTAL_LEN] = "";
	unsigned char broad_cast[UDO_MAC_ADDR_LEN] = "";
	udo_link_layer link_layer_spool;
	udo_link_layer_init(&link_layer_spool);
	self->op = UDO_ARP_REQUEST;
		
	memset(broad_cast, 255, UDO_MAC_ADDR_LEN);
	udo_link_layer_setdst(&link_layer_spool, broad_cast);
	udo_link_layer_setsrc(&link_layer_spool, udo_adapter_mac(self->adapter));
	udo_link_layer_settype(&link_layer_spool, UDO_LINK_TYPE_ARP);
	udo_link_layer_serialize(&link_layer_spool, packet, 0);

	udo_arp_setdia(self,ip);
	udo_arp_setsia(self, udo_adapter_ip(self->adapter));
	udo_arp_setsma(self, udo_adapter_mac(self->adapter));

	udo_arp_serialize(self, packet, UDO_LINK_LAYER_LEN);
	udo_adapter_send(self->adapter, packet, UDO_ARP_TOTAL_LEN);
}

