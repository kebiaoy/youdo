#ifndef udo_host_h__
#define udo_host_h__
#include "udo_ip.h"
#include "../common/core/udo_list.h"

typedef struct udo_host
{
	udo_ip ip;
	unsigned char mac[UDO_MAC_ADDR_LEN];
	udo_list_item item;
}udo_host;

void udo_host_init(udo_host* self);

void udo_host_setip(udo_host* self, udo_ip* ip);

udo_ip* udo_host_getip(udo_host* self);

void udo_host_setmac(udo_host* self, unsigned char* mac);

void udo_host_copy(udo_host* self, udo_host* other);

unsigned char* udo_host_getmac(udo_host* self);

int udo_host_equal(udo_host* self, udo_host* other);

void udo_host_term(udo_host* self);





#endif // udo_host_h__
