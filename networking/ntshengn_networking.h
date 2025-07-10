#pragma once
#include "../Common/networking/ntshengn_networking_interface.h"
#include <forward_list>
#include <memory>

namespace NtshEngn {

	class Networking : public NetworkingInterface {
	public:
		void init();
		void update();
		void destroy();

		ServerSocketInterface* createServerSocket(uint16_t port, NetworkType networkType);
		ClientSocketInterface* createClientSocket(NetworkType networkType);

		void closeServerSocket(ServerSocketInterface* serverSocket);
		void closeClientSocket(ClientSocketInterface* clientSocket);

	private:
		ServerSocketInterface* createServerSocketUDP(uint16_t port);
		ServerSocketInterface* createServerSocketTCP(uint16_t port);

		ClientSocketInterface* createClientSocketUDP();
		ClientSocketInterface* createClientSocketTCP();

	private:
		std::forward_list<std::unique_ptr<ServerSocketInterface>> m_servers;
		std::forward_list<std::unique_ptr<ClientSocketInterface>> m_clients;
	};

}