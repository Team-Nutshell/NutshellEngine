#include "ntshengn_networking_client_socket.h"
#include "../utils/ntshengn_core_defines.h"
#if defined(NTSHENGN_OS_WINDOWS)
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#elif defined(NTSHENGN_OS_LINUX)
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#define SOCKET_ERROR -1
#define WSAGetLastError() errno
#define WSAECONNRESET ECONNRESET
#define WSAEWOULDBLOCK EWOULDBLOCK
#define closesocket close
#endif
#include <array>

#define BUFFER_SIZE 65536

void NtshEngn::ClientSocket::connectToServer(const std::string& ipAddress, uint16_t port) {
	if (m_networkType == NetworkType::UDP) {
		connectToServerUDP(ipAddress, port);
	}
	else if (m_networkType == NetworkType::TCP) {
		connectToServerTCP(ipAddress, port);
	}
}

void NtshEngn::ClientSocket::sendDataToServer(const void* data, size_t dataSize) {
	if (m_connectedServer.ipAddress == "") {
		NTSHENGN_NETWORKING_WARNING("[CLIENT] Client is not connected to any server.");
		return;
	}

	if (m_networkType == NetworkType::UDP) {
		sendDataToServerUDP(data, dataSize);
	}
	else if (m_networkType == NetworkType::TCP) {
		sendDataToServerTCP(data, dataSize);
	}
}

void NtshEngn::ClientSocket::setServerDisconnectCallback(std::function<void()> callback) {
	m_serverDisconnectCallback = callback;
}

void NtshEngn::ClientSocket::setDataReceivedCallback(std::function<void(void*, size_t)> callback) {
	m_dataReceivedCallback = callback;
}

const NtshEngn::ConnectedServer& NtshEngn::ClientSocket::getConnectedServer() {
	return m_connectedServer;
}

void NtshEngn::ClientSocket::update() {
	if (m_networkType == NetworkType::UDP) {
		updateUDP();
	}
	else if (m_networkType == NetworkType::TCP) {
		updateTCP();
	}
}

void NtshEngn::ClientSocket::destroy() {
	if (m_networkType == NetworkType::UDP) {
		destroyUDP();
	}
	else if (m_networkType == NetworkType::TCP) {
		destroyTCP();
	}
}

void NtshEngn::ClientSocket::connectToServerUDP(const std::string& ipAddress, uint16_t port) {
	m_connectedServer.ipAddress = ipAddress;
	m_connectedServer.port = port;

	uint16_t connectData = NTSHENGN_NETWORKING_HEADER_CONNECTION;
	sendDataToServer(&connectData, sizeof(uint16_t));

	NTSHENGN_NETWORKING_INFO("[CLIENT / UDP] Registered information on server at IP address " + ipAddress + ".");
}

void NtshEngn::ClientSocket::connectToServerTCP(const std::string& ipAddress, uint16_t port) {
	sockaddr_in sockaddrIn;
	sockaddrIn.sin_family = AF_INET;
	sockaddrIn.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &sockaddrIn.sin_addr.s_addr);

	if (connect(m_socket, reinterpret_cast<sockaddr*>(&sockaddrIn), static_cast<int>(sizeof(sockaddr_in))) == 0) {
		NTSHENGN_NETWORKING_INFO("[CLIENT / TCP] Connected to server at IP address " + ipAddress + " and port " + std::to_string(port) + ".");
	}
	else {
		if (WSAGetLastError() == WSAEWOULDBLOCK) {
			NTSHENGN_NETWORKING_INFO("[CLIENT / TCP] Attempting to connect to server at IP address " + ipAddress + " and port " + std::to_string(port) + ".");
		}
		else {
			NTSHENGN_NETWORKING_WARNING("[CLIENT / TCP] Could not connect to server at IP address " + ipAddress + " and port " + std::to_string(port) + ".");
		}
	}

	m_connectedServer.ipAddress = ipAddress;
	m_connectedServer.port = port;
}

void NtshEngn::ClientSocket::sendDataToServerUDP(const void* data, size_t dataSize) {
	sockaddr_in serverSockaddr;
	serverSockaddr.sin_family = AF_INET;
	serverSockaddr.sin_port = htons(m_connectedServer.port);
	inet_pton(AF_INET, m_connectedServer.ipAddress.c_str(), &serverSockaddr.sin_addr.s_addr);
	if (sendto(m_socket, static_cast<const char*>(data), static_cast<int>(dataSize), 0, reinterpret_cast<sockaddr*>(&serverSockaddr), static_cast<int>(sizeof(sockaddr_in))) == SOCKET_ERROR) {
		NTSHENGN_NETWORKING_WARNING("[CLIENT / UDP] Error sending data to server.");
	}
}

void NtshEngn::ClientSocket::sendDataToServerTCP(const void* data, size_t dataSize) {
	if (send(m_socket, static_cast<const char*>(data), static_cast<int>(dataSize), 0) < 0) {
		if (WSAGetLastError() == WSAECONNRESET) {
			if (m_serverDisconnectCallback) {
				m_serverDisconnectCallback();
			}

			m_connectedServer.ipAddress = "";

			NTSHENGN_NETWORKING_WARNING("[CLIENT / TCP] Server has disconnected.");
		}
	}
}

void NtshEngn::ClientSocket::updateUDP() {
	std::array<char, BUFFER_SIZE> buffer;

	sockaddr_in serverSockaddr;
	serverSockaddr.sin_family = AF_INET;
	serverSockaddr.sin_port = htons(m_connectedServer.port);
	inet_pton(AF_INET, m_connectedServer.ipAddress.c_str(), &serverSockaddr.sin_addr.s_addr);
	socklen_t sockaddrSize = static_cast<int>(sizeof(sockaddr_in));
	int receive = recvfrom(m_socket, buffer.data(), BUFFER_SIZE, 0, reinterpret_cast<sockaddr*>(&serverSockaddr), &sockaddrSize);
	if ((receive != 0) && (receive != SOCKET_ERROR)) {
		uint16_t disconnectHeader = (static_cast<uint16_t>(buffer[1]) << 8) + static_cast<uint8_t>(buffer[0]);
		if ((receive == sizeof(uint16_t)) && (disconnectHeader == NTSHENGN_NETWORKING_HEADER_DISCONNECTION)) {
			// Server disconnection
			if (m_serverDisconnectCallback) {
				m_serverDisconnectCallback();
			}

			NTSHENGN_NETWORKING_INFO("[CLIENT / UDP] Server has disconnected.");
		}
		else {
			// Receive data
			if (m_dataReceivedCallback) {
				m_dataReceivedCallback(buffer.data(), receive);
			}
		}
	}
}

void NtshEngn::ClientSocket::updateTCP() {
	std::array<char, BUFFER_SIZE> buffer;

	if (m_connectedServer.ipAddress != "") {
		int receive = recv(m_socket, buffer.data(), BUFFER_SIZE, 0);
		if ((receive != 0) && (receive != SOCKET_ERROR)) {
			// Receive data
			if (m_dataReceivedCallback) {
				m_dataReceivedCallback(buffer.data(), receive);
			}
		}
		else {
			if ((receive == 0) || (WSAGetLastError() == WSAECONNRESET)) {
				// Server disconnection
				if (m_serverDisconnectCallback) {
					m_serverDisconnectCallback();
				}

				m_connectedServer.ipAddress = "";

				NTSHENGN_NETWORKING_INFO("[CLIENT / TCP] Client has been disconnected from server.");
			}
		}
	}
}

void NtshEngn::ClientSocket::destroyUDP() {
	uint16_t disconnectData = NTSHENGN_NETWORKING_HEADER_DISCONNECTION;
	sendDataToServer(&disconnectData, sizeof(uint16_t));

	if (closesocket(m_socket) == 0) {
		NTSHENGN_NETWORKING_INFO("[CLIENT / UDP] Client has been closed.");
	}
	else {
		NTSHENGN_NETWORKING_INFO("[CLIENT / UDP] Could not close client.");
	}
}

void NtshEngn::ClientSocket::destroyTCP() {
	if (closesocket(m_socket) == 0) {
		NTSHENGN_NETWORKING_INFO("[CLIENT / TCP] Client has been closed.");
	}
	else {
		NTSHENGN_NETWORKING_INFO("[CLIENT / TCP] Could not close client.");
	}
}