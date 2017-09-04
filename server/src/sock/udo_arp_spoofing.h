#ifndef udo_arp_spoofing_h__
#define udo_arp_spoofing_h__

struct udo_adapter;
struct udo_server_dns;


typedef struct udo_arp_spoofing
{
	int is_run;
	struct udo_adapter* adapter;
}udo_arp_spoofing;


void udo_arp_spoofing_init(udo_arp_spoofing* self,struct udo_adapter* adapter);

void udo_arp_spoofing_start(udo_arp_spoofing* self);

void udo_arp_spoofing_stop(udo_arp_spoofing* self);

void udo_arp_spoofing_term(udo_arp_spoofing* self);

#endif // udo_arp_spoofing_h__
