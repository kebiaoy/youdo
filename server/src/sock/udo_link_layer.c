#include <WinSock2.h>
#include "../common/string/udo_num.h"
#include "../common/error/udo_error.h"
#include "udo_link_layer.h"


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
	memcpy(self->dst_addr, dst, UDO_MAC_ADDR_LEN);
}

void udo_link_layer_setsrc(udo_link_layer* self, unsigned char* src)
{
	udo_assert(src);
	memcpy(self->src_addr, src, UDO_MAC_ADDR_LEN);
}

void udo_link_layer_settype(udo_link_layer* self, int type)
{
	self->type = type;
}

void udo_link_layer_term(udo_link_layer* self)
{

}


