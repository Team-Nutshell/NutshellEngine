#include "ntshengn_networking_server_socket.h"
#include "../utils/ntshengn_core_defines.h"
#include "../../external/Common/utils/ntshengn_defines.h"
#if defined(NTSHENGN_OS_WINDOWS)
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#elif defined(NTSHENGN_OS_LINUX)
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#define WSAGetLastError() errno
#define WSAECONNRESET ECONNRESET
#define WSAEWOULDBLOCK EWOULDBLOCK
#define closesocket close
#endif
#include <array>
#include <string>
#include <cstring>
#include <limits>

#define BUFFER_SIZE 65536

void NtshEngn::ServerSocket::sendDataToClient(ConnectedClientID clientID, void* data, size_t dataSize) {
	NTSHENGN_ASSERT(m_connectedClients.find(clientID) != m_connectedClients.end());

	if (m_networkType == NetworkType::UDP) {
		sendDataToClientUDP(clientID, data, dataSize);
	}
	else if (m_networkType == NetworkType::TCP) {
		sendDataToClientTCP(clientID, data, dataSize);
	}
}

void NtshEngn::ServerSocket::setClientConnectCallback(std::function<void(ConnectedClientID)> callback) {
	m_clientConnectCallback = callback;
}

void NtshEngn::ServerSocket::setClientDisconnectCallback(std::function<void(ConnectedClientID)> callback) {
	m_clientDisconnectCallback = callback;
}

void NtshEngn::ServerSocket::setDataReceivedCallback(std::function<void(ConnectedClientID, void*, size_t)> callback) {
	m_dataReceivedCallback = callback;
}

void NtshEngn::ServerSocket::update() {
	if (m_networkType == NetworkType::UDP) {
		updateUDP();
	}
	else if (m_networkType == NetworkType::TCP) {
		updateTCP();
	}
}

void NtshEngn::ServerSocket::destroy() {
	if (m_networkType == NetworkType::UDP) {
		destroyUDP();
	}
	else if (m_networkType == NetworkType::TCP) {
		destroyTCP();
	}
}

void NtshEngn::ServerSocket::sendDataToClientUDP(ConnectedClientID clientID, void* data, size_t dataSize) {
	sockaddr_in clientSockaddr;
	clientSockaddr.sin_family = AF_INET;
	clientSockaddr.sin_port = htons(m_connectedClients[clientID].port);
	inet_pton(AF_INET, m_connectedClients[clientID].ipAddress.c_str(), &clientSockaddr.sin_addr.s_addr);
	if (sendto(m_socket, static_cast<char*>(data), static_cast<int>(dataSize), 0, reinterpret_cast<sockaddr*>(&clientSockaddr), static_cast<int>(sizeof(sockaddr_in))) == SOCKET_ERROR) {
		NTSHENGN_NETWORKING_WARNING("[SERVER / UDP] Error sending data to client.");
	}
}

void NtshEngn::ServerSocket::sendDataToClientTCP(ConnectedClientID clientID, void* data, size_t dataSize) {
	if (send(m_connectedClients[clientID].socket, static_cast<char*>(data), static_cast<int>(dataSize), 0) < 0) {
		if (WSAGetLastError() == WSAECONNRESET) {
			if (m_clientDisconnectCallback) {
				m_clientDisconnectCallback(clientID);
			}

			m_connectedClients.erase(clientID);

			NTSHENGN_NETWORKING_INFO("[SERVER / TCP] Client with ClientID " + std::to_string(clientID) + " has disconnected.");
		}
	}
}

void NtshEngn::ServerSocket::updateUDP() {
	std::array<char, BUFFER_SIZE> buffer;

	sockaddr_in newSockaddr;
	socklen_t sockaddrSize = static_cast<int>(sizeof(sockaddr_in));
	int receive = recvfrom(m_socket, buffer.data(), BUFFER_SIZE, 0, reinterpret_cast<sockaddr*>(&newSockaddr), &sockaddrSize);
	while ((receive != 0) && (receive != SOCKET_ERROR)) {
		std::array<char, 15> clientIPArray;
		const char* ipAddress = inet_ntop(AF_INET, &newSockaddr.sin_addr, clientIPArray.data(), clientIPArray.size());
		std::string clientIP(clientIPArray.data(), strlen(ipAddress));
		uint16_t clientPort = ntohs(newSockaddr.sin_port);

		ConnectedClientID currentConnectedClientID = std::numeric_limits<ConnectedClientID>::max();
		for (auto& client : m_connectedClients) {
			if ((client.second.ipAddress == clientIP) && (client.second.port == clientPort)) {
				currentConnectedClientID = client.first;
				break;
			}
		}

		// New connection
		if (currentConnectedClientID == std::numeric_limits<ConnectedClientID>::max()) {
			ConnectedClient connectedClient;
			connectedClient.ipAddress = clientIP;
			connectedClient.port = clientPort;
			m_connectedClients[m_connectedClientID++] = connectedClient;

			currentConnectedClientID = m_connectedClientID - 1;

			if (m_clientConnectCallback) {
				m_clientConnectCallback(currentConnectedClientID);
			}

			NTSHENGN_NETWORKING_INFO("[SERVER / UDP] New client with ConnectedClientID " + std::to_string(currentConnectedClientID) + " connected to server.");
		}

		uint16_t deconnectionHeader = (static_cast<uint16_t>(buffer[1]) << 8) + static_cast<uint8_t>(buffer[0]);
		if ((receive == sizeof(uint16_t)) && (deconnectionHeader == 0xDEC0)) {
			// Client disconnect
			if (m_clientDisconnectCallback) {
				m_clientDisconnectCallback(currentConnectedClientID);
			}

			m_connectedClients.erase(currentConnectedClientID);

			NTSHENGN_NETWORKING_INFO("[SERVER / UDP] Client with ConnectedClientID " + std::to_string(currentConnectedClientID) + " disconnected from server.");
		}
		else {
			// Receive data
			if (m_dataReceivedCallback) {
				m_dataReceivedCallback(currentConnectedClientID, buffer.data(), receive);
			}
		}

		receive = recvfrom(m_socket, buffer.data(), BUFFER_SIZE, 0, reinterpret_cast<sockaddr*>(&newSockaddr), &sockaddrSize);
	}
}

void NtshEngn::ServerSocket::updateTCP() {
	std::array<char, BUFFER_SIZE> buffer;

	// New connection
	sockaddr_in newClientSockaddr;
	socklen_t sockaddrSize = static_cast<int>(sizeof(sockaddr_in));
	SOCKET newClientSocket = accept(m_socket, reinterpret_cast<sockaddr*>(&newClientSockaddr), &sockaddrSize);
	while ((newClientSocket != INVALID_SOCKET)) {
		ConnectedClient connectedClient;
		connectedClient.socket = newClientSocket;
		connectedClient.port = ntohs(newClientSockaddr.sin_port);
		m_connectedClients[m_connectedClientID++] = connectedClient;

		if (m_clientConnectCallback) {
			m_clientConnectCallback(m_connectedClientID - 1);
		}

		NTSHENGN_NETWORKING_INFO("[SERVER / TCP] New client with ConnectedClientID " + std::to_string(m_connectedClientID - 1) + " connected to server.");

		newClientSocket = accept(m_socket, reinterpret_cast<sockaddr*>(&newClientSockaddr), &sockaddrSize);
	}

	for (auto it = m_connectedClients.begin(); it != m_connectedClients.end(); ) {
		ConnectedClient client = it->second;
		bool clientDisconnected = false;

		SOCKET clientSocket = client.socket;
		int receive = recv(clientSocket, buffer.data(), BUFFER_SIZE, 0);
		if ((receive != 0) && (receive != SOCKET_ERROR)) {
			// Receive data
			if (m_dataReceivedCallback) {
				m_dataReceivedCallback(it->first, buffer.data(), receive);
			}
		}
		else {
			if ((receive == 0) || (WSAGetLastError() == WSAECONNRESET)) {
				// Client disconnect
				if (m_clientDisconnectCallback) {
					m_clientDisconnectCallback(it->first);
				}

				NTSHENGN_NETWORKING_INFO("[SERVER / TCP] Client with ConnectedClientID " + std::to_string(it->first) + " disconnected from server.");

				it = m_connectedClients.erase(it);
				clientDisconnected = true;
			}
		}

		if (!clientDisconnected) {
			it++;
		}
	}
}

void NtshEngn::ServerSocket::destroyUDP() {
	for (auto& it : m_connectedClients) {
		uint16_t disconnectData = 0xDEC0;
		sendDataToClient(it.first, &disconnectData, sizeof(uint16_t));
	}

	if (closesocket(m_socket) == 0) {
		NTSHENGN_NETWORKING_INFO("[SERVER] Server on port " + std::to_string(m_port) + " closed.");
	}
	else {
		NTSHENGN_NETWORKING_INFO("[SERVER] Could not close server on port " + std::to_string(m_port) + ".");
	}
}

void NtshEngn::ServerSocket::destroyTCP() {
	if (closesocket(m_socket) == 0) {
		NTSHENGN_NETWORKING_INFO("[SERVER] Server on port " + std::to_string(m_port) + " closed.");
	}
	else {
		NTSHENGN_NETWORKING_INFO("[SERVER] Could not close server on port " + std::to_string(m_port) + ".");
	}
}