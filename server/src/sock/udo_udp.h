#ifndef udo_udp_h__
#define udo_udp_h__

#include "udo_ip.h"

typedef struct udo_udp
{
	unsigned short len;
	unsigned short src_port;
	unsigned short dst_port;
	unsigned short cksum;
}udo_udp;


void udo_udp_init(udo_udp* self);

void udo_udp_deserialize(udo_udp* self, unsigned char* packet, int start_offset);

void udo_udp_serialize(udo_udp* self, unsigned char* packet, int start_offset);

unsigned short udo_udp_getdstport(udo_udp* self);

void udo_udp_term(udo_udp* self);

#endif // udo_udp_h__
