#include "udo_host.h"


void udo_host_init(udo_host* self)
{
	udo_ip_init(&self->ip);
	udo_list_item_init(&self->item);
	memset(self->mac, 0, UDO_MAC_ADDR_LEN);
}

void udo_host_setip(udo_host* self, udo_ip* ip)
{
	udo_ip_copy(&self->ip, ip);
}

udo_ip* udo_host_getip(udo_host* self)
{
	return &self->ip;
}

void udo_host_setmac(udo_host* self, unsigned char* mac)
{
	memcpy(self->mac, mac, UDO_MAC_ADDR_LEN);
}

unsigned char* udo_host_getmac(udo_host* self)
{
	return self->mac;
}

void udo_host_term(udo_host* self)
{
	udo_ip_term(&self->ip);
	udo_list_item_term(&self->item);
	memset(self->mac, 0, UDO_MAC_ADDR_LEN);
}

int udo_host_equal(udo_host* self, udo_host* other)
{
	for (int i = 0; i < UDO_MAC_ADDR_LEN; ++i)
	{
		if (self->mac[i] != other->mac[i])
			return UDO_NOT_EQUAL;
	}
	if (!udo_ip_equal(&self->ip, &other->ip))
	{
		return UDO_NOT_EQUAL;
	}
	return UDO_EQUAL;
}

void udo_host_copy(udo_host* self, udo_host* other)
{
	udo_ip_copy(&self->ip, &other->ip);
	memcpy(self->mac, other->mac, UDO_MAC_ADDR_LEN);
}

