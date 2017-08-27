#include <WinSock2.h>
#include <stdio.h>
#include "common/error/udo_error.h"
#include "common/config/udo_config.h"
#include "http/udo_http_req.h"
#include "http/udo_http_rep.h"


void SendResponse(int client)
{
	FILE* f=NULL;
	udo_config config;
 	char buf[1024] = "HTTP/1.0 200 ok\r\n"
		"sssssss\r\n"
 		"Content-Type: text/html\r\n"
		"\r\n";
	char html[4096] = "";
	udo_config_init(&config);
	fopen_s(&f, udo_config_get_res(&config, "HomePage.html"), "r+b");
	fread_s(html, 4096, 4096, 1, f);
	printf("%s", html);
	send(client, buf, strlen(buf), 0);
	
	send(client, html, strlen(html), 0);
	fclose(f);
}


int main()
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}
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
		sendBuf = udo_http_rep_serialize(&rep);
		send(fd1, sendBuf, strlen(sendBuf),0);
		closesocket(fd1);
		
	}
	system("pause");
	return 0;
}