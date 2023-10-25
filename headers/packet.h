#pragma once

#include "platform_init.h"

#include <cstdint>
#include <string>

namespace Weaver
{

	class Packet
	{
	public:
		char* buffer;
		uint32_t size;

		uint32_t content_size;

		sockaddr_in remote_addr;

		Packet(uint32_t sz);
		Packet(const char* buf, uint32_t sz);
		Packet(const char* buf, uint32_t sz, uint16_t port, const char* ip_string);
		Packet(const Packet& p);
		Packet(const Packet* p);
		~Packet();

		void clear();

		// funcitions to get and set ip/port to and from string.
		void set_remote_addr(uint16_t port, const char* ip_string);
		// data serialization here?


	};

}