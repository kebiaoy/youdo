#include <WinSock2.h>
#include "../common/error/udo_error.h"
#include "udo_server_web.h"


static void udo_server_dns(udo_server_web* self);


void udo_server_web_init(udo_server_web* self, char* domain_name)
{
	int dns_len = 0;
	udo_assert(domain_name);
	dns_len = strlen(domain_name);
	udo_assert(dns_len < UDO_SERVER_DNS_LEN);
	memcpy(self->domain_name, domain_name, dns_len);
	self->domain_name[dns_len] = '\0';
	udo_server_dns(self);
}

void udo_server_web_loop(udo_server_web* self)
{

}

void udo_server_web_term(udo_server_web* self)
{

}

void udo_server_dns(udo_server_web* self)
{
	struct hostent* pHosten=NULL;
	int err;
	pHosten=gethostbyname(self->domain_name);
	err = WSAGetLastError();
	if (err != WSAHOST_NOT_FOUND || !pHosten)
	{
		udo_system_error(0);
	}
	if (pHosten)
	{
		return;
	}
// 	sethostname
// 	
}

