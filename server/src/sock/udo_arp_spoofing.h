#ifndef udo_arp_spoofing_h__
#define udo_arp_spoofing_h__

#include "../common/core/udo_list.h"

struct udo_adapter;
struct udo_server_dns;
struct udo_host;

typedef struct udo_arp_spoofing
{
	int is_run;
	struct udo_adapter* adapter;
	udo_list list;
}udo_arp_spoofing;


void udo_arp_spoofing_init(udo_arp_spoofing* self,struct udo_adapter* adapter);

void udo_arp_spoofing_start(udo_arp_spoofing* self);

int udo_arp_spoofing_isrun(udo_arp_spoofing* self);

void udo_arp_spoofing_run(udo_arp_spoofing* self);

void udo_arp_spoofing_stop(udo_arp_spoofing* self);

void udo_arp_spoofing_exec(udo_arp_spoofing* self);

void udo_arp_spoofing_addhost(udo_arp_spoofing* self, struct udo_host* host);

void udo_arp_spoofing_term(udo_arp_spoofing* self);

#endif // udo_arp_spoofing_h__
