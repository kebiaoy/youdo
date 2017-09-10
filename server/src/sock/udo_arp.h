#ifndef udo_arp_h__
#define udo_arp_h__

#include "udo_ip.h"

#define UDO_ARP_REQUEST 1
#define UDO_ARP_RESPONSE 2

#define UDO_ARP_TOTAL_LEN 60

struct udo_adapter;

typedef struct udo_arp
{
	unsigned short hardware_type;
	unsigned short procotol_type;
	unsigned char hardware_len;
	unsigned char procotol_len;
	unsigned short op;
	unsigned char src_mac_addr[UDO_MAC_ADDR_LEN];
	udo_ip src_ip_addr;
	unsigned char dst_mac_addr[UDO_MAC_ADDR_LEN];
	udo_ip dst_ip_addr;
	struct udo_adapter* adapter;
}udo_arp;

void udo_arp_init(udo_arp* self, struct udo_adapter* adapter);

void udo_arp_setop(udo_arp* self,int op);

int udo_arp_getop(udo_arp* self);

void udo_arp_setsma(udo_arp* self, unsigned char* sma);

unsigned char* udo_arp_getsma(udo_arp* self);

void udo_arp_setsia(udo_arp* self, udo_ip* sia);

udo_ip* udo_arp_getsia(udo_arp* self);

void udo_arp_setdma(udo_arp* self, unsigned char* dma);

unsigned char* udo_arp_getdma(udo_arp* self);

void udo_arp_setdia(udo_arp* self, udo_ip* dia);

udo_ip* udo_arp_getdia(udo_arp* self);

void udo_arp_deserialize(udo_arp* self, unsigned char* packet, int start_offset);

void udo_arp_serialize(udo_arp* self, unsigned char* packet, int start_offset);

void udo_arp_resonse(udo_arp* self);

void udo_arp_request(udo_arp* self, udo_ip* ip);

void udo_arp_term(udo_arp* self);


#endif // udo_arp_h__
