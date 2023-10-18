#include "weaver.h"

#include <iostream>

namespace Weaver
{

	uint32_t NetWeave::_sock_intitiated = 0;

	NetWeave::NetWeave()
	{
		_recv_sock = NULL;
		_send_sock = NULL;
		
		if (NetWeave::_sock_intitiated)
		{
			NetWeave::_sock_intitiated++;
			return;
		}
		if (!sock_init()) 
		{
			std::cout << get_sock_error() << '\n';
		}
		NetWeave::_sock_intitiated++;
	}

	NetWeave::~NetWeave()
	{
		NetWeave::_sock_intitiated--;
		if (!NetWeave::_sock_intitiated)
		{
			sock_cleanup();
		}
	}


	void NetWeave::start_receiver(uint16_t port, std::function<void(Packet*, NetWeave*)> cb)
	{
		if (_recv_sock)
		{
			std::cout << "receiver already running. call stop first before starting again.\n";
			exit(10);
		}
		if (_send_sock)
		{
			if (_send_sock->_port == port)
			{
				std::cout << "port already in use by sender.\n";
				exit(10);
			}
		}
		

		_recv_callback = cb;
		_recv_sock = new UDPSocket(port);

		_callback_handler_running = true;
		_callback_handler_thread = std::thread(_callback_handler_loop, this);
		_receiver_running = true;
		_receiver_thread = std::thread(_receiver_loop, this);

	}

	void NetWeave::stop_receiver()
	{
	}

	void NetWeave::start_sender(uint16_t port)
	{
		if (_send_sock)
		{
			std::cout << "sender already running. call stop first before starting again.\n";
			exit(10);
		}
		if (_recv_sock)
		{
			if (_recv_sock->_port == port)
			{
				std::cout << "port already in use by sender.\n";
				exit(10);
			}
		}

		_send_sock = new UDPSocket(port);

	}

	void NetWeave::send_data(Packet* pck)
	{
	}

	void NetWeave::stop_sender()
	{
	}

	void NetWeave::_callback_handler_loop()
	{
		while (_callback_handler_running)
		{

		}
	}

	void NetWeave::_receiver_loop()
	{
		while (_receiver_running)
		{

		}
	}

	void NetWeave::_sender_loop()
	{
		while (_sender_running)
		{

		}
	}

}


