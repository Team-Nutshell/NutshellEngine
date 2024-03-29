#pragma once
#include "ntshengn_networking_common.h"
#if defined(NTSHENGN_OS_WINDOWS)
#include <winsock2.h>
#elif defined(NTSHENGN_OS_LINUX)
#define SOCKET int
#endif
#include <functional>
#include <string>

namespace NtshEngn {

	struct ConnectedServer {
		std::string ipAddress = ""; // UDP
		uint16_t port; // UDP & TCP
	};

	class ClientSocket {
	public:
		ClientSocket(SOCKET socket, NetworkType networkType):
			m_socket(socket), m_networkType(networkType) {}

		void connectToServer(const std::string& ipAddress, uint16_t port);
		void sendDataToServer(const void* data, size_t dataSize);
		void setServerDisconnectCallback(std::function<void()> callback);
		void setDataReceivedCallback(std::function<void(void*, size_t)> callback);

		const ConnectedServer& getConnectedServer();

		void update();
		void destroy();

	private:
		void connectToServerUDP(const std::string& ipAddress, uint16_t port);
		void connectToServerTCP(const std::string& ipAddress, uint16_t port);

		void sendDataToServerUDP(const void* data, size_t dataSize);
		void sendDataToServerTCP(const void* data, size_t dataSize);

		void updateUDP();
		void updateTCP();

		void destroyUDP();
		void destroyTCP();

	private:
		SOCKET m_socket;
		NetworkType m_networkType;
		ConnectedServer m_connectedServer;

		std::function<void()> m_serverDisconnectCallback;
		std::function<void(void*, size_t)> m_dataReceivedCallback;
	};

}