#pragma once

#include "platform_init.h"
#include "packet.h"

#include <cstdint>

namespace Weaver
{
	// UDP socket class
	class UDPSocket
	{
	private:
		UDPSocket(uint16_t port = 0, uint32_t recv_timeout = 1000);
		~UDPSocket();

		void set_timeout(uint32_t msec);
		void receive(Weaver::Packet* pck);
		void send(Weaver::Packet* pck);

		SOCKET _sock_fd;
		uint16_t _port;
		sockaddr_in _self_addr;


		friend class NetWeave;
	};

}
