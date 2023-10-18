#pragma once

#include <iostream>

#ifdef _WIN32

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#else

#include <arpa/inet.h>
#include <errorno.h>
#include <netinet.h>
#include <sys/socket.h>

#endif	// _WIN32


inline bool sock_init()
{

#ifdef _WIN32
	WSADATA wsadata;
	int res = WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (res != 0)
	{
		std::cerr << "wsa startup failed, error code: " << res << std::endl;
		return false;
	}
#endif	// _WIN32
	return true;

}

inline int get_sock_error()
{

#ifdef _WIN32
	return WSAGetLastError();
#else
	return errno;
#endif

}

inline void sock_cleanup()
{

#ifdef _WIN32
	WSACleanup();
#endif	// _WIN32

}

