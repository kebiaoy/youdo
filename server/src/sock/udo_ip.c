#include <WinSock2.h>
#include "udo_ip.h"

void udo_ip_init(udo_ip* self)
{
	self->ip = 0;
}

void udo_ip_setformat(udo_ip* self, unsigned char* ip)
{
	self->ip = inet_addr(ip);
}

unsigned char* udo_ip_getformat(udo_ip* self)
{
	struct in_addr addr;
	addr.S_un.S_addr = self->ip;
	return inet_ntoa(addr);
}

void udo_ip_setnet(udo_ip* self, unsigned char* ip)
{
	self->ip = *((unsigned int*)ip);
}

unsigned char* udo_ip_getnet(udo_ip* self)
{
	return (unsigned char*)(&self->ip);
}

void udo_ip_copy(udo_ip* self, udo_ip* other)
{
	self->ip = other->ip;
}

void udo_ip_term(udo_ip* self)
{
	self->ip = 0;
}

int udo_ip_equal(udo_ip* self, udo_ip* other)
{
	return self->ip == other->ip;
}



