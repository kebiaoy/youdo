#include <winsock2.h>
#include "udo_udp.h"


void udo_udp_init(udo_udp* self)
{
	self->len = 0;
	self->src_port = 0;
	self->dst_port = 0;
	self->cksum = 0;
}

void udo_udp_deserialize(udo_udp* self, unsigned char* packet, int start_offset)
{
	unsigned char* t_packt = packet + start_offset;
	self->src_port = htons(*(unsigned short*)t_packt);
	t_packt += 2;
	self->dst_port = htons(*(unsigned short*)t_packt);
	t_packt += 2;
	self->len = htons(*(unsigned short*)t_packt);
	t_packt += 2;
	self->cksum = htons(*(unsigned short*)t_packt);
	t_packt += 2;
}

void udo_udp_serialize(udo_udp* self, unsigned char* packet, int start_offset)
{

}

void udo_udp_term(udo_udp* self)
{

}

unsigned short udo_udp_getdstport(udo_udp* self)
{
	return self->dst_port;
}

