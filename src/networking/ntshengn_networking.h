#pragma once
#include "ntshengn_networking_server_socket.h"
#include "ntshengn_networking_client_socket.h"
#include <unordered_map>
#include <functional>
#include <forward_list>
#include <string>

namespace NtshEngn {

	class Networking {
	public:
		void init();
		void update();
		void destroy();

		ServerSocket* createServerSocket(uint16_t port, NetworkType networkType);
		ClientSocket* createClientSocket(NetworkType networkType);

		void closeServerSocket(ServerSocket* serverSocket);
		void closeClientSocket(ClientSocket* clientSocket);

	private:
		ServerSocket* createServerSocketUDP(uint16_t port);
		ServerSocket* createServerSocketTCP(uint16_t port);

		ClientSocket* createClientSocketUDP();
		ClientSocket* createClientSocketTCP();

	private:
		std::forward_list<ServerSocket> m_servers;
		std::forward_list<ClientSocket> m_clients;
	};

}