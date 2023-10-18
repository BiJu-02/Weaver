#include "packet.h"

#include <cstring>
#include <iostream>

namespace Weaver
{

	Packet::Packet(uint32_t sz)
		: size(sz), buffer(new char[sz])
	{ }

	Packet::Packet(const char* buf, uint32_t sz)
		: size(sz), buffer(new char[sz])
	{
		std::memcpy(buffer, buf, sz);
	}

	Packet::Packet(const Packet& p)
		: size(p.size), buffer(new char[p.size])
	{
		std::memcpy(buffer, p.buffer, p.size);
	}

	Packet::Packet(const Packet* p)
	{
		if (p)
		{
			size = p->size;
			buffer = new char[p->size];
			std::memcpy(buffer, p->buffer, p->size);
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

}
