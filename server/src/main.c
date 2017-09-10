#include <WinSock2.h>
#include <stdio.h>
#include <pcap.h>
#include "common/error/udo_error.h"
#include "common/config/udo_config.h"
#include "http/udo_http_req.h"
#include "http/udo_http_rep.h"
#include "sock/udo_server_dns.h"
#include <IPHlpApi.h>


int main()
{
	//getpacket();
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}
	udo_server_dns dns_server;
	udo_server_dns_init(&dns_server, "www.udo.com");
	udo_server_dns_start(&dns_server);
	SOCKET fd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	SOCKADDR_IN SockAddr,clentaddr;
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	SockAddr.sin_port = htons(80);
	if (bind(fd, (SOCKADDR*)&SockAddr, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		udo_system_error();
	}
	if (listen(fd, 30) == SOCKET_ERROR)
	{
		udo_system_error();
	}
	while (1)
	{
		int len = sizeof(SOCKADDR);
		char buf[1024] = "";
		char* sendBuf = NULL;
		SOCKET fd1 = accept(fd, (SOCKADDR*)&clentaddr, &len);
		recv(fd1, buf, 1024, 0);
		udo_http_req req;
		udo_http_req_init(&req);
		udo_http_req_deserialize(&req, buf, 1024);
		udo_http_rep rep;
		udo_http_rep_init(&rep, &req);
		if (udo_http_rep_serialize(&rep) == UDO_HTTP_REP_SERIALIZE_FAILED)
		{
			closesocket(fd1);
			continue;
		}

		sendBuf = udo_http_rep_deserialize(&rep);
		send(fd1, sendBuf, udo_http_rep_length(&rep),0);
		closesocket(fd1);
		udo_http_rep_term(&rep);
		udo_http_req_term(&req);
	}
	system("pause");
	return 0;
}