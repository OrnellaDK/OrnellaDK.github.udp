#pragma once
#ifndef NETWORK_H
#define NETWORK_H
//#define WIN32

#ifdef _WIN32
#pragma warning(disable: 4668 4820 4710)
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSDKVer.h> 
#include <winsock2.h>
#include <ws2tcpip.h>
#include <synchapi.h>
#include <time.h>
#pragma warning(default: 4668 4820 4710)
#define sleep Sleep

#pragma comment(lib, "Ws2_32.lib")

typedef long int ssize_t;
typedef unsigned short int uint16_t;

#else
#ifdef __linux__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#else

//#error not available on your platform

#endif
#endif

static void initNetwork(void)
{
#ifdef _WIN32
	WSADATA wsa;
	int err = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (err < 0)
	{
		printf("WSAStartup failed !\n");
		exit(EXIT_FAILURE);
	}
#endif
}

static void cleanNetwork(void)
{
#ifdef _WIN32
	WSACleanup();
#endif
}
#endif