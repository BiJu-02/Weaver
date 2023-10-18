#pragma once

#include "platform_init.h"

#include <cstdint>

namespace Weaver
{
	// UDP socket class
	class UDPSocket
	{
	public:
		sockaddr_in remote_addr;

		UDPSocket(uint16_t port = 0);
		//void bind
	private:
		uint32_t _sock_fd;
		uint16_t _port;
		sockaddr_in _self_addr;

		friend class NetWeave;
	};

}
