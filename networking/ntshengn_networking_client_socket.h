#pragma once
#include "../Common/networking/ntshengn_networking_client_socket_interface.h"
#include "../Common/networking/ntshengn_networking_common.h"
#include <functional>
#include <string>

namespace NtshEngn {

	class ClientSocket : public ClientSocketInterface {
	public:
		ClientSocket(Socket socket, NetworkType networkType);

		void update();
		void destroy();

		void connectToServer(const std::string& ipAddress, uint16_t port);

		void sendDataToServer(const void* data, size_t dataSize);

		void setServerDisconnectCallback(std::function<void()> callback);
		void setDataReceivedCallback(std::function<void(void*, size_t)> callback);

		const ConnectedServer& getConnectedServer();

	private:
		void updateUDP();
		void updateTCP();

		void destroyUDP();
		void destroyTCP();

		void connectToServerUDP(const std::string& ipAddress, uint16_t port);
		void connectToServerTCP(const std::string& ipAddress, uint16_t port);

		void sendDataToServerUDP(const void* data, size_t dataSize);
		void sendDataToServerTCP(const void* data, size_t dataSize);

	private:
		Socket m_socket;
		NetworkType m_networkType;
		ConnectedServer m_connectedServer;

		std::function<void()> m_serverDisconnectCallback;
		std::function<void(void*, size_t)> m_dataReceivedCallback;
	};

}