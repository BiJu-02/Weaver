#pragma once

#include "packet.h"
#include "socket.h"

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
#include <queue>

namespace Weaver
{

	class NetWeave
	{
	public:
		NetWeave();
		~NetWeave();
		
		void start_receiver(uint16_t port, std::function<void(Packet*, NetWeave*)> cb);
		void stop_receiver();

		void start_sender(uint16_t port);
		void send_data(Packet* pck);
		void stop_sender();

	private:
		static uint32_t _sock_intitiated;

		UDPSocket* _recv_sock;
		UDPSocket* _send_sock;
		// remote socket list for tcp...

		std::function<void(Weaver::Packet*, Weaver::NetWeave*)> _recv_callback;

		std::thread _callback_handler_thread;
		std::thread _receiver_thread;
		std::thread _sender_thread;

		std::atomic<bool> _callback_handler_running;
		std::atomic<bool> _receiver_running;
		std::atomic<bool> _sender_running;
		
		std::mutex _receiver_mtx;
		std::mutex _sender_mtx;

		std::condition_variable _receiver_cv;
		std::condition_variable _sender_cv;

		std::queue<Packet*> _receiver_data_q;
		std::queue<Packet*> _sender_data_q;
		
		void _callback_handler_loop();
		void _receiver_loop();
		void _sender_loop();
	};


}