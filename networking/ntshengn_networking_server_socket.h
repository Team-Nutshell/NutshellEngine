#pragma once
#include "../../Common/networking/ntshengn_networking_server_socket_interface.h"
#include "ntshengn_networking_common.h"
#include <string>
#include <unordered_map>
#include <functional>

namespace NtshEngn {

	class ServerSocket : public ServerSocketInterface {
	public:
		ServerSocket(Socket socket, uint16_t port, NetworkType networkType);

		void update();
		void destroy();

		void sendDataToClient(ConnectedClientID clientID, const void* data, size_t dataSize);

		void setClientConnectCallback(std::function<void(ConnectedClientID)> callback);
		void setClientDisconnectCallback(std::function<void(ConnectedClientID)> callback);
		void setDataReceivedCallback(std::function<void(ConnectedClientID, void*, size_t)> callback);

		uint16_t getPort();
		const std::unordered_map<ConnectedClientID, ConnectedClient>& getConnectedClients();

	private:
		void updateUDP();
		void updateTCP();

		void destroyUDP();
		void destroyTCP();

		void sendDataToClientUDP(ConnectedClientID clientID, const void* data, size_t dataSize);
		void sendDataToClientTCP(ConnectedClientID clientID, const void* data, size_t dataSize);

	private:
		Socket m_socket;
		uint16_t m_port;
		NetworkType m_networkType;
		std::unordered_map<ConnectedClientID, ConnectedClient> m_connectedClients;
		ConnectedClientID m_connectedClientID = 0;

		std::function<void(ConnectedClientID)> m_clientConnectCallback;
		std::function<void(ConnectedClientID)> m_clientDisconnectCallback;
		std::function<void(ConnectedClientID, void*, size_t)> m_dataReceivedCallback;
	};

}