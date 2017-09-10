#include <WinSock2.h>
#include "udo_dns.h"

void udo_dns_init(udo_dns* self)
{
	memset(self->domain_name, 0, 64);
}

void udo_dns_deserialize(udo_dns* self, unsigned char* packet, int start_offset)
{
	unsigned char* t_packet = packet + start_offset;
	unsigned char* t_domain = self->domain_name;
	self->identification = htons(*(unsigned short*)t_packet);
	t_packet += 2;
	self->sign = htons(*(unsigned short*)t_packet);
	t_packet += 2;
	self->question_count = htons(*(unsigned short*)t_packet);
	t_packet += 2;
	self->res_count = htons(*(unsigned short*)t_packet);
	t_packet += 2;
	self->authorization_count = htons(*(unsigned short*)t_packet);
	t_packet += 2;
	self->extra_count = htons(*(unsigned short*)t_packet);
	t_packet += 2;
	while (1)
	{
		unsigned char len = t_packet[0];
		t_packet += 1;
		for (int i = 0; i < len; ++i)
		{
			t_domain[i] = t_packet[i];
		}
		t_domain += len;
		t_packet += len;
		if (t_packet[0] != 0)
		{
			t_domain[0] = '.';
			t_domain += 1;
		}
		else
		{
			break;
		}
	}
	t_packet++;
}

void udo_dns_term(udo_dns* self)
{

}

