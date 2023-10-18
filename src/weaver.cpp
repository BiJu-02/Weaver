
#include "weaver.h"

#include "platform_init.h"

#include <iostream>

namespace Weaver
{

	uint32_t NetWeave::_sock_intitiated = 0;

	NetWeave::NetWeave()
	{
		_receiver_sock = nullptr;
		_sender_sock = nullptr;
		
		if (NetWeave::_sock_intitiated)
		{
			NetWeave::_sock_intitiated++;
			return;
		}
		if (!sock_init()) 
		{
			exit(-101);
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


	void NetWeave::test_func()
	{
		std::cout << "so far so good\n";
	}


	void NetWeave::start_receiver(uint16_t port, std::function<void(Packet*, NetWeave*)> cb)
	{
		if (_receiver_sock)
		{
			throw std::runtime_error("receiver already running. call stop first before starting again.\n");
		}
		if (_sender_sock)
		{
			if (_sender_sock->_port == port)
			{
				throw std::runtime_error("port already in use by sender.\n");
			}
		}
		

		_recv_callback = cb;
		_receiver_sock = new UDPSocket(port);

		_callback_handler_running = true;
		_callback_handler_thread = std::thread(&NetWeave::_callback_handler_loop, this);
		_receiver_running = true;
		_receiver_thread = std::thread(&NetWeave::_receiver_loop, this);

	}

	void NetWeave::stop_receiver()
	{
		_receiver_running = false;
		_receiver_thread.join();
		_callback_handler_running = false;
		_callback_handler_cv.notify_one();
		_callback_handler_thread.join();
		// close _receiver_sock and delete it

	}

	void NetWeave::start_sender(uint16_t port)
	{
		if (_sender_sock)
		{
			throw std::runtime_error("sender already running. call stop first before starting again.\n");
		}
		if (_receiver_sock)
		{
			if (_receiver_sock->_port == port)
			{
				throw std::runtime_error("port already in use by sender.\n");
			}
		}

		_sender_sock = new UDPSocket(port);

		_sender_running = true;
		_sender_thread = std::thread(&NetWeave::_sender_loop, this);

	}

	void NetWeave::stop_sender()
	{
		_sender_running = false;
		_sender_thread.join();
		// close _sender_sock and delete it

	}

	void NetWeave::send_data(const Packet& pck)
	{
		Packet* temp_packet = new Packet(pck);
		std::unique_lock<std::mutex> lock(_sender_q_mtx);
		_sender_data_q.push(temp_packet);
		lock.unlock();
		_sender_cv.notify_one();
	}

	void NetWeave::_callback_handler_loop()
	{
		std::unique_lock<std::mutex> lock(_receiver_q_mtx, std::defer_lock);
		std::queue<Packet*> internal_q;
		Packet* temp_packet;
		while (_callback_handler_running)
		{
			lock.lock();
			_callback_handler_cv.wait(lock, [&]() { return !_receiver_data_q.empty() || !_callback_handler_running; });
			if (_callback_handler_running)
			{
				while (!_receiver_data_q.empty())
				{
					internal_q.push(_receiver_data_q.front());
					_receiver_data_q.pop();
				}
				lock.unlock();
				while (!internal_q.empty())
				{
					temp_packet = internal_q.front();
					internal_q.pop();
					_recv_callback(temp_packet, this);
					if (temp_packet)
					{ delete temp_packet; }
				}
			}
			else
			{
				lock.unlock();
				break;
			}
		}

		lock.lock();
		while (!_receiver_data_q.empty())
		{
			temp_packet = _receiver_data_q.front();
			_receiver_data_q.pop();
			delete temp_packet;
		}
		lock.unlock();

		while (!internal_q.empty())
		{
			temp_packet = internal_q.front();
			internal_q.pop();
			delete temp_packet;
		}
	}

	void NetWeave::_receiver_loop()
	{
		std::unique_lock<std::mutex> lock(_receiver_q_mtx, std::defer_lock);
		Packet* temp_packet;
		while (_receiver_running)
		{
			temp_packet = new Packet(_max_buffer_size);

			////// socket recv code start //////



			////// socket recv code ends //////
			
			lock.lock();
			_receiver_data_q.push(temp_packet);
			lock.unlock();
			_callback_handler_cv.notify_one();

		}
		// if internal queue is required...clean up here before returning

	}

	void NetWeave::_sender_loop()
	{
		std::unique_lock<std::mutex> lock(_receiver_q_mtx, std::defer_lock);
		std::queue<Packet*> internal_q;
		Packet* temp_packet;
		while (_sender_running)
		{
			lock.lock();
			_sender_cv.wait(lock, [&]() { return !_sender_data_q.empty() || !_sender_running; });
			if (_sender_running)
			{
				while (!_sender_data_q.empty())
				{
					internal_q.push(_sender_data_q.front());
					_sender_data_q.pop();
				}
				lock.unlock();
				while (!internal_q.empty())
				{
					temp_packet = internal_q.front();
					internal_q.pop();

					////// socket send code start //////



					////// socket send code ends //////

					delete temp_packet;
				}
			}
			else
			{
				lock.unlock();
				break;
			}
		}

		lock.lock();
		while (!_sender_data_q.empty())
		{
			temp_packet = _sender_data_q.front();
			_sender_data_q.pop();
			delete temp_packet;
		}
		lock.unlock();

		while (!internal_q.empty())
		{
			temp_packet = internal_q.front();
			internal_q.pop();
			delete temp_packet;
		}
	}

}


