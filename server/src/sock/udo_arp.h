#ifndef udo_arp_h__
#define udo_arp_h__

#define UDO_ARP_REQUEST 1
#define UDO_ARP_RESPONSE 2

typedef struct udo_arp
{
	unsigned short hardware_type;
	unsigned short procotol_type;
	unsigned char hardware_len;
	unsigned char procotol_len;
	unsigned short op;
	unsigned char src_mac_addr[UDO_MAC_ADDR_LEN];
	unsigned char src_ip_addr[UDO_IP_ADDR_LEN];
	unsigned char dst_mac_addr[UDO_MAC_ADDR_LEN];
	unsigned char dst_ip_addr[UDO_IP_ADDR_LEN];
}udo_arp;

void udo_arp_init(udo_arp* self);

void udo_arp_setop(udo_arp* self,int op);

int udo_arp_getop(udo_arp* self);

void udo_arp_setsma(udo_arp* self, unsigned char* sma);

unsigned char* udo_arp_getsma(udo_arp* self);

void udo_arp_setsia(udo_arp* self, unsigned char* sia);

unsigned char* udo_arp_getsia(udo_arp* self);

void udo_arp_setdma(udo_arp* self, unsigned char* dma);

unsigned char* udo_arp_getdma(udo_arp* self);

void udo_arp_setdia(udo_arp* self, unsigned char* dia);

unsigned char* udo_arp_getdia(udo_arp* self);

void udo_arp_deserialize(udo_arp* self, unsigned char* packet, int start_offset);

void udo_arp_serialize(udo_arp* self, unsigned char* packet, int start_offset);

void udo_arp_term(udo_arp* self);


#endif // udo_arp_h__
