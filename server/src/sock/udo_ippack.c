#include "udo_ippack.h"
#include <WinSock2.h>

#define UDO_IPPACK_VERSION_MASK 0xF0

#define UDO_IPPACK_HEADERLEN_MASK 0x0F

#define UDO_IPPACK_FLAGS_MASK 0xE0

#define UDO_IPPACK_FRAGOFFSET_MASK 0x1F

void udo_ippack_init(udo_ippack* self)
{
	self->version = 0;;
	self->header_len = 0;
	self->tos = 0;
	self->total_len = 0;
	self->identification = 0;
	self->flags=0;
	self->frag_offset=0;
	self->ttl;
	self->protocol;
	self->cksum=0;
	udo_ip_init(&self->src_ip);
	udo_ip_init(&self->dst_ip);
}

void udo_ippack_deserialize(udo_ippack* self, unsigned char* packet, int start_offset)
{
	unsigned char* t_packt = packet + start_offset;
	self->version = (t_packt[0] & UDO_IPPACK_VERSION_MASK) >> 4;
	self->header_len = t_packt[0] & UDO_IPPACK_HEADERLEN_MASK;
	t_packt += 1;
	self->tos = t_packt[0];
	t_packt += 1;
	self->total_len = htons(*(unsigned short*)t_packt);
	t_packt += 2;
	self->identification = htons(*(unsigned short*)t_packt);
	t_packt += 2;
	self->flags = (t_packt[0] & UDO_IPPACK_FLAGS_MASK) >> 5;
	self->frag_offset = (t_packt[0] & UDO_IPPACK_FRAGOFFSET_MASK) * 256 + t_packt[1];
	t_packt += 2;
	self->ttl = t_packt[0];
	t_packt += 1;
	self->protocol = t_packt[0];
	t_packt += 1;
	self->cksum = htons(*(unsigned short*)t_packt);
	t_packt += 2;
	udo_ip_setnet(&self->src_ip, t_packt);
	t_packt += UDO_IP_ADDR_LEN;
	udo_ip_setnet(&self->dst_ip, t_packt);
	t_packt += UDO_IP_ADDR_LEN;
}

void udo_ippack_serialize(udo_ippack* self, unsigned char* packet, int start_offset)
{

}

void udo_ippack_term(udo_ippack* self)
{
	udo_ip_term(&self->src_ip);
	udo_ip_term(&self->dst_ip);
}

unsigned char udo_ippack_getprotocol(udo_ippack* self)
{
	return self->protocol;
}

unsigned int udo_ippack_dataoffset(udo_ippack* self)
{
	return self->header_len * 4;
}
