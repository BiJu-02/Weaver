#pragma once

#include <cstdint>

namespace Weaver
{

	class Packet
	{
	public:
		char* buffer;
		uint32_t size;

		Packet(uint32_t sz);
		Packet(const char* buf, uint32_t sz);
		Packet(const Packet& p);
		Packet(const Packet* p);
		~Packet();
	};

}