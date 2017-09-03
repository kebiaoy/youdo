#ifndef udo_link_layer_h__
#define udo_link_layer_h__


#define UDO_LINK_TYPE_IP 0x0800
#define UDO_LINK_TYPE_ARP 0x0806
#define UDO_LINK_TYPE_RARP 0x8035

typedef struct udo_link_layer
{
	unsigned char dst_addr[UDO_MAC_ADDR_LEN];
	unsigned char src_addr[UDO_MAC_ADDR_LEN];
	unsigned short type;
}udo_link_layer;


void udo_link_layer_init(udo_link_layer* self);

void udo_link_layer_deserialize(udo_link_layer* self,char* packet);

void udo_link_layer_serialize(udo_link_layer* self,unsigned char* packet,int start_offset);

unsigned char* udo_link_layer_getdst(udo_link_layer* self);

void udo_link_layer_setdst(udo_link_layer* self, unsigned char* dst);

unsigned char* udo_link_layer_getsrc(udo_link_layer* self);

void udo_link_layer_setsrc(udo_link_layer* self, unsigned char* src);

int udo_link_layer_gettype(udo_link_layer* self);

void udo_link_layer_settype(udo_link_layer* self,int type);

void udo_link_layer_term(udo_link_layer* self);

#endif // udo_link_layer_h__
