#ifndef udo_ippack_h__
#define udo_ippack_h__

#include "udo_ip.h"

typedef struct udo_ippack
{
	unsigned char version;
	unsigned char header_len;
	unsigned char tos;
	unsigned short total_len;
	unsigned short identification;
	unsigned char flags;
	unsigned short frag_offset;
	unsigned char ttl;
	unsigned char protocol;
	unsigned short cksum;
	udo_ip src_ip;
	udo_ip dst_ip;
}udo_ippack;

void udo_ippack_init(udo_ippack* self);

void udo_ippack_deserialize(udo_ippack* self, unsigned char* packet, int start_offset);

void udo_ippack_serialize(udo_ippack* self, unsigned char* packet, int start_offset);

unsigned int udo_ippack_dataoffset(udo_ippack* self);

unsigned char udo_ippack_getprotocol(udo_ippack* self);

void udo_ippack_term(udo_ippack* self);


#endif // udo_ippack_h__
