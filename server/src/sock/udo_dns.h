#ifndef udo_dns_h__
#define udo_dns_h__


typedef struct udo_dns
{
	unsigned short identification;
	unsigned short sign;
	unsigned short question_count;
	unsigned short res_count;
	unsigned short authorization_count;
	unsigned short extra_count;
	unsigned char domain_name[64];
}udo_dns;

void udo_dns_init(udo_dns* self);

void udo_dns_deserialize(udo_dns* self, unsigned char* packet, int start_offset);

void udo_dns_term(udo_dns* self);

#endif // udo_dns_h__
