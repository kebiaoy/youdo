#ifndef udo_adapter_h__
#define udo_adapter_h__

#define UDO_IF_TYPE_ETHERNET 6
#define UDO_ADAPTER_NAME_LENGTH 64

//adapter 需要关闭适配器 在析构的时候

typedef struct udo_adapter
{
 	unsigned char mac_addr[UDO_IP_ADDR_FORMAT_LEN];
	unsigned char ip[UDO_IP_ADDR_FORMAT_LEN];
	unsigned char gateway_ip[UDO_IP_ADDR_FORMAT_LEN];
	unsigned char mask[UDO_IP_ADDR_FORMAT_LEN];
 	int mac_len;
 	int adapter_type;
	char adapter_name[UDO_ADAPTER_NAME_LENGTH];
	void *adhandle;
}udo_adapter;

void udo_adapter_init(udo_adapter* self, int adapter_type);
unsigned char* udo_adapter_mac(udo_adapter* self);
int udo_adapter_mlen(udo_adapter* self);
unsigned char* udo_adapter_ip(udo_adapter* self);
char* udo_adapter_name(udo_adapter* self);
unsigned char* udo_adapter_gateway(udo_adapter* self);
unsigned char* udo_adapter_mask(udo_adapter* self);
void udo_adapter_send(udo_adapter* self, char* packet, int packet_len);
int udo_adapter_rpacket(udo_adapter* self, unsigned char** packet, int *packet_len);
void udo_adapter_term(udo_adapter* self);
#endif // udo_adpter_h__
