
#include "socket.h"

#include <thread>

namespace Weaver
{

	UDPSocket::UDPSocket(uint16_t port, uint32_t recv_timeout) : _port(port)
	{
		if ((_sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
		{
			std::cerr << "socket creation failed, error code: " << get_sock_error() << std::endl;
			sock_cleanup();
			exit(-102);
		}
		if (port)
		{
			memset((void*)&_self_addr, 0, sizeof(_self_addr));
			_self_addr.sin_family = AF_INET;
			_self_addr.sin_port = htons(port);
			_self_addr.sin_addr.s_addr = INADDR_ANY;
			if (bind(_sock_fd, (sockaddr*)&_self_addr, sizeof(sockaddr)) == SOCKET_ERROR)
			{
				std::cerr << "socket bind failed, error code: " << get_sock_error() << std::endl;
				sock_cleanup();
				exit(-103);
			}
		}
		setsockopt(_sock_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&recv_timeout, sizeof(recv_timeout));
	}

	UDPSocket::~UDPSocket()
	{
		closesocket(_sock_fd);
	}

	void UDPSocket::set_timeout(uint32_t msec)
	{
		setsockopt(_sock_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&msec, sizeof(msec));
	}

	void UDPSocket::receive(Weaver::Packet* pck)
	{
		pck->clear();
		int32_t addr_len = sizeof(sockaddr);
		int32_t bytes_sent = recvfrom(_sock_fd, pck->buffer, pck->size, 0, (sockaddr*)&(pck->remote_addr), &addr_len);
		if (bytes_sent == SOCKET_ERROR)
		{
			if (timeout_occured())
			{
				std::this_thread::yield();
			}
			else
			{
				std::cerr << "error recieving data: " << get_sock_error() << std::endl;
				exit(-104);
			}
		}
		else
		{
			pck->content_size = bytes_sent;
		}
	}

	void UDPSocket::send(Weaver::Packet* pck)
	{
		uint32_t send_start = 0;
		uint32_t bytes_sent;
		while (send_start < pck->content_size)
		{
			bytes_sent = sendto(_sock_fd, pck->buffer + send_start, pck->content_size - send_start, 0, (sockaddr*)&(pck->remote_addr), sizeof(sockaddr));
			if (bytes_sent == SOCKET_ERROR)
			{
				std::cerr << "error while sending data: " << get_sock_error() << std::endl;
				exit(-105);
			}
			send_start += bytes_sent;
		}
		
	}

}