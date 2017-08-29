#include <WinSock2.h>
#include <stdio.h>
#include "common/error/udo_error.h"
#include "common/config/udo_config.h"
#include "http/udo_http_req.h"
#include "http/udo_http_rep.h"
#include <pcre2.h>


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
	PCRE2_SPTR pattern = "Get";
	PCRE2_SPTR subject = "Get \image\res\logo";
	int errornumber;
	PCRE2_SIZE erroroffset;
	pcre2_code *re;
	PCRE2_SIZE *ovector;
	size_t subject_length;
	pcre2_match_data *match_data;
	int rc;
	re = pcre2_compile(pattern, PCRE2_SIZE_MAX, 0, &errornumber, &erroroffset, NULL);
	if (re == NULL)
	{
		PCRE2_UCHAR buffer[256];
		pcre2_get_error_message(errornumber, buffer, sizeof(buffer));
		printf("PCRE2 compilation failed at offset %d: %s\n", (int)erroroffset,buffer);
	}
	match_data = pcre2_match_data_create_from_pattern(re, NULL);
	subject_length = strlen(subject);
	rc = pcre2_match(re,subject,subject_length,0,0,match_data,NULL);

	if (rc < 0)
	{
		switch (rc)
		{
			case PCRE2_ERROR_NOMATCH: 
				printf("No match\n"); 
				break;
			default: 
				printf("Matching error %d\n", rc); 
				break;
		}
		pcre2_match_data_free(match_data);   /* Release memory used for the match */
		pcre2_code_free(re);                 /* data and the compiled pattern. */
		return 1;
	}

	ovector = pcre2_get_ovector_pointer(match_data);
	printf("Match succeeded at offset %d\n", (int)ovector[0]);
	if (rc == 0) 
	{
		printf("ovector only has room for %d captured substrings\n", rc - 1);
	}

	for (int i = 0; i < rc; i++) 
	{
		char *substring_start = subject + ovector[2 * i];
		int substring_length = ovector[2 * i + 1] - ovector[2 * i];
		printf("%2d: %.*s\n", i, substring_length, substring_start);
	}
	pcre2_match_data_free(match_data);
	pcre2_code_free(re);
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