#include "socket.h"

namespace Weaver
{

	UDPSocket::UDPSocket(uint16_t port) : _port(port)
	{
		if ((_sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
		{
			std::cerr << "socket creation failed, error code: " << get_sock_error() << std::endl;
			exit(-102);
		}
	}

}