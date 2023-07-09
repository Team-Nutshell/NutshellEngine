#pragma once
#include "ntshengn_networking_common.h"
#if defined(NTSHENGN_OS_WINDOWS)
#include <winsock2.h>
#elif defined(NTSHENGN_OS_LINUX)
#define SOCKET int
#endif
#include <unordered_map>
#include <functional>

namespace NtshEngn {

	struct ConnectedClient {
		SOCKET socket; // TCP
		std::string ipAddress; // UDP
		uint16_t port; // UDP & TCP
	};

	class ServerSocket {
	public:
		ServerSocket(SOCKET socket, uint16_t port, NetworkType networkType):
			m_socket(socket), m_port(port), m_networkType(networkType) {}

		void sendDataToClient(ConnectedClientID clientID, const void* data, size_t dataSize);
		void setClientConnectCallback(std::function<void(ConnectedClientID)> callback);
		void setClientDisconnectCallback(std::function<void(ConnectedClientID)> callback);
		void setDataReceivedCallback(std::function<void(ConnectedClientID, void*, size_t)> callback);

		const std::unordered_map<ConnectedClientID, ConnectedClient>& getConnectedClients();

		void update();
		void destroy();

	private:
		void sendDataToClientUDP(ConnectedClientID clientID, const void* data, size_t dataSize);
		void sendDataToClientTCP(ConnectedClientID clientID, const void* data, size_t dataSize);

		void updateUDP();
		void updateTCP();

		void destroyUDP();
		void destroyTCP();

	private:
		SOCKET m_socket;
		uint16_t m_port;
		NetworkType m_networkType;
		std::unordered_map<ConnectedClientID, ConnectedClient> m_connectedClients;
		ConnectedClientID m_connectedClientID = 0;

		std::function<void(ConnectedClientID)> m_clientConnectCallback;
		std::function<void(ConnectedClientID)> m_clientDisconnectCallback;
		std::function<void(ConnectedClientID, void*, size_t)> m_dataReceivedCallback;
	};

}