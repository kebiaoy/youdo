#ifndef udo_adapter_h__
#define udo_adapter_h__
#include <ipifcons.h>


#define UDO_ADAPTER_ADDR_LENGTH 32
#define UDO_ADAPTER_NAME_LENGTH 64
typedef struct udo_adapter
{
	UCHAR mac_addr[UDO_ADAPTER_ADDR_LENGTH];
	UCHAR ip[UDO_ADAPTER_ADDR_LENGTH];
	int mac_len;
	int adapter_type;
	char adapter_name[UDO_ADAPTER_NAME_LENGTH];
}udo_adapter;

void udo_adapter_init(udo_adapter* self, int adapter_type);
char* udo_adapter_mac(udo_adapter* self);
int udo_adapter_mlen(udo_adapter* self);
char* udo_adapter_ip(udo_adapter* self);
char* udo_adapter_name(udo_adapter* self);
void udo_adapter_term(udo_adapter* self);
#endif // udo_adpter_h__
