#include <WinSock2.h>
#include "udo_link_layer.h"
#include "../common/error/udo_error.h"

void udo_link_layer_init(udo_link_layer* self)
{
	memset(self->dst_addr, 0, UDO_MAC_ADDR_LEN);
	memset(self->src_addr, 0, UDO_MAC_ADDR_LEN);
}

unsigned char* udo_link_layer_getdst(udo_link_layer* self)
{
	return self->dst_addr;
}

unsigned char* udo_link_layer_getsrc(udo_link_layer* self)
{
	return self->src_addr;
}

int udo_link_layer_gettype(udo_link_layer* self)
{
	return self->type;
}

void udo_link_layer_deserialize(udo_link_layer* self, char* packet)
{
	unsigned char* t_packet = packet;
	memcpy(self->dst_addr, t_packet, UDO_MAC_ADDR_LEN);
	memcpy(self->src_addr, t_packet + UDO_MAC_ADDR_LEN, UDO_MAC_ADDR_LEN);
	self->type = htons(*((unsigned short*)(t_packet + (2 * UDO_MAC_ADDR_LEN))));
}


void udo_link_layer_serialize(udo_link_layer* self, unsigned char* packet, int start_offset)
{
	unsigned char* t_packet = &packet[start_offset];
	unsigned short* type;
	memcpy(t_packet,self->dst_addr,UDO_MAC_ADDR_LEN);
	t_packet += UDO_MAC_ADDR_LEN;
	memcpy(t_packet, self->src_addr, UDO_MAC_ADDR_LEN);
	t_packet += UDO_MAC_ADDR_LEN;
	type = (unsigned short*)t_packet;
	type[0] = htons(self->type);
}

void udo_link_layer_setdst(udo_link_layer* self, unsigned char* dst)
{
	udo_assert(dst);
	int j = 0;
	for (int i = 0; i < UDO_MAC_ADDR_FORMAT_LEN; ++i)
	{
		if (dst[i] == UDO_MAC_ADDR_FORMAT_CHAR)
		{
			j++;
			continue;
		}
		self->dst_addr[j] = self->dst_addr[j] * 16 + (dst[i] - '0');
	}
}

void udo_link_layer_setsrc(udo_link_layer* self, unsigned char* src)
{
	udo_assert(src);
	int j = 0;
	for (int i = 0; i < UDO_MAC_ADDR_FORMAT_LEN; ++i)
	{
		if (src[i] == UDO_MAC_ADDR_FORMAT_CHAR)
		{
			j++;
			continue;
		}
		self->src_addr[j] = self->src_addr[j] * 16 + (src[i] - '0');
	}
}

void udo_link_layer_settype(udo_link_layer* self, int type)
{
	self->type = type;
}

void udo_link_layer_term(udo_link_layer* self)
{

}


