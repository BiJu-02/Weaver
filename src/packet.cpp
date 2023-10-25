
#include "packet.h"

#include <cstring>
#include <iostream>

namespace Weaver
{

	Packet::Packet(uint32_t sz)
		: size(sz), buffer(new char[sz])
	{
		clear();
	}

	Packet::Packet(const char* buf, uint32_t sz)
		: size(sz), buffer(new char[sz]), content_size(sz)
	{
		std::memcpy(buffer, buf, sz);
	}

	Packet::Packet(const char* buf, uint32_t sz, uint16_t port, const char* ip_string)
		: size(sz), buffer(new char[sz]), content_size(sz)
	{
		std::memcpy(buffer, buf, sz);
		memset((void*)&remote_addr, 0, sizeof(remote_addr));
		remote_addr.sin_family = AF_INET;
		remote_addr.sin_port = htons(port);
		inet_pton(AF_INET, ip_string, (void*)&(remote_addr.sin_addr));
	}

	Packet::Packet(const Packet& p)
		: size(p.size), buffer(new char[p.size]), content_size(p.content_size)
	{
		std::memcpy(buffer, p.buffer, p.size);
		std::memcpy((void*)&remote_addr, (void*)&(p.remote_addr), sizeof(sockaddr));
	}

	Packet::Packet(const Packet* p)
	{
		if (p)
		{
			size = p->size;
			buffer = new char[p->size];
			content_size = p->content_size;
			std::memcpy(buffer, p->buffer, p->size);
			std::memcpy((void*)&remote_addr, (void*)&(p->remote_addr), sizeof(sockaddr));
		}
		else
		{
			throw std::invalid_argument("null pointer passed in constructor\n");
		}
	}

	Packet::~Packet()
	{
		delete[] buffer;
	}

	void inline Packet::clear()
	{
		std::memset(buffer, 0, sizeof(buffer));
		content_size = 0;
	}

	void Packet::set_remote_addr(uint16_t port, const char* ip_string)
	{
		memset((void*)&remote_addr, 0, sizeof(remote_addr));
		remote_addr.sin_family = AF_INET;
		remote_addr.sin_port = htons(port);
		inet_pton(AF_INET, ip_string, (void*)&(remote_addr.sin_addr));
	}

}
