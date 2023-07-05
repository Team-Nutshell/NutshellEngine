#include "ntshengn_networking.h"
#include "../utils/ntshengn_core_defines.h"
#if defined(NTSHENGN_OS_WINDOWS)
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#elif defined(NTSHENGN_OS_LINUX)
#include <netinet/in.h>
#include <fcntl.h>
#define INVALID_SOCKET -1
#endif

void NtshEngn::Networking::init() {
#if defined(NTSHENGN_OS_WINDOWS)
	WSADATA data;
	if (WSAStartup(MAKEWORD(2, 0), &data) != 0) {
		NTSHENGN_NETWORKING_WARNING("Could not start WSA.");
	}
#endif
}

void NtshEngn::Networking::update() {
	// Servers
	for (ServerSocket& server : m_servers) {
		server.update();
	}

	// Clients
	for (ClientSocket& client : m_clients) {
		client.update();
	}
}

void NtshEngn::Networking::destroy() {
	for (ServerSocket& server : m_servers) {
		server.destroy();
	}
	for (ClientSocket& client : m_clients) {
		client.destroy();
	}

#if defined(NTSHENGN_OS_WINDOWS)
	if (WSACleanup() != 0) {
		NTSHENGN_NETWORKING_WARNING("Could not cleanup WSA.");
	}
#endif
}

NtshEngn::ServerSocket* NtshEngn::Networking::createServerSocket(uint16_t port, NetworkType networkType) {
	if (networkType == NetworkType::UDP) {
		return createServerSocketUDP(port);
	}
	else if (networkType == NetworkType::TCP) {
		return createServerSocketTCP(port);
	}

	return nullptr;
}

NtshEngn::ClientSocket* NtshEngn::Networking::createClientSocket(NetworkType networkType) {
	if (networkType == NetworkType::UDP) {
		return createClientSocketUDP();
	}
	else if (networkType == NetworkType::TCP) {
		return createClientSocketTCP();
	}

	return nullptr;
}

void NtshEngn::Networking::closeServerSocket(ServerSocket* serverSocket) {
	serverSocket->destroy();

	std::forward_list<ServerSocket>::iterator prev = m_servers.before_begin();
	for (std::forward_list<ServerSocket>::iterator it = m_servers.begin(); it != m_servers.end(); it++) {
		if (serverSocket == &(*it)) {
			m_servers.erase_after(prev);
			return;
		}

		prev = it;
	}
}

void NtshEngn::Networking::closeClientSocket(ClientSocket* clientSocket) {
	clientSocket->destroy();

	std::forward_list<ClientSocket>::iterator prev = m_clients.before_begin();
	for (std::forward_list<ClientSocket>::iterator it = m_clients.begin(); it != m_clients.end(); it++) {
		if (clientSocket == &(*it)) {
			m_clients.erase_after(prev);
			return;
		}

		prev = it;
	}
}

NtshEngn::ServerSocket* NtshEngn::Networking::createServerSocketUDP(uint16_t port) {
	sockaddr_in sockaddrIn;
	sockaddrIn.sin_family = AF_INET;
	sockaddrIn.sin_port = htons(port);
	sockaddrIn.sin_addr.s_addr = htonl(INADDR_ANY);

	SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock == INVALID_SOCKET) {
		NTSHENGN_NETWORKING_WARNING("[SERVER / UDP] Error creating socket for server.");
	}
#if defined(NTSHENGN_OS_WINDOWS)
	unsigned long nonBlockingSocket = 1;
	int nonBlocking = ioctlsocket(sock, FIONBIO, &nonBlockingSocket);
#elif defined(NTSHENGN_OS_LINUX)
	int nonBlocking = fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK);
#endif
	if (nonBlocking != 0) {
		NTSHENGN_NETWORKING_WARNING("[SERVER / UDP] Error while making the server socket non-blocking.");
	}

	if (bind(sock, reinterpret_cast<sockaddr*>(&sockaddrIn), static_cast<int>(sizeof(sockaddr_in))) == 0) {
		NTSHENGN_NETWORKING_INFO("[SERVER / UDP] Server opened on port " + std::to_string(port) + ".");
	}
	else {
		NTSHENGN_NETWORKING_WARNING("[SERVER / UDP] Could not bind socket to port " + std::to_string(port) + " when opening server.");
	}

	ServerSocket serverSocket(sock, port, NetworkType::UDP);
	m_servers.push_front(serverSocket);

	return &m_servers.front();
}

NtshEngn::ServerSocket* NtshEngn::Networking::createServerSocketTCP(uint16_t port) {
	sockaddr_in sockaddrIn;
	sockaddrIn.sin_family = AF_INET;
	sockaddrIn.sin_port = htons(port);
	sockaddrIn.sin_addr.s_addr = htonl(INADDR_ANY);

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		NTSHENGN_NETWORKING_WARNING("[SERVER / TCP] Error creating socket for server.");
	}
#if defined(NTSHENGN_OS_WINDOWS)
	unsigned long nonBlockingSocket = 1;
	int nonBlocking = ioctlsocket(sock, FIONBIO, &nonBlockingSocket);
#elif defined(NTSHENGN_OS_LINUX)
	int nonBlocking = fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK);
#endif
	if (nonBlocking != 0) {
		NTSHENGN_NETWORKING_WARNING("[SERVER / TCP] Error while making the server socket non-blocking.");
	}

	if (bind(sock, reinterpret_cast<sockaddr*>(&sockaddrIn), static_cast<int>(sizeof(sockaddr_in))) == 0) {
		NTSHENGN_NETWORKING_INFO("[SERVER / TCP] Server opened on port " + std::to_string(port) + ".");
	}
	else {
		NTSHENGN_NETWORKING_WARNING("[SERVER / TCP] Could not bind socket to port " + std::to_string(port) + " when opening server.");
	}

	if (listen(sock, SOMAXCONN) == 0) {
		NTSHENGN_NETWORKING_INFO("[SERVER / TCP] Listening on socket.");
	}

	ServerSocket serverSocket(sock, port, NetworkType::TCP);
	m_servers.push_front(serverSocket);

	return &m_servers.front();
}

NtshEngn::ClientSocket* NtshEngn::Networking::createClientSocketUDP() {
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock == INVALID_SOCKET) {
		NTSHENGN_NETWORKING_WARNING("[CLIENT / UDP] Error creating socket for client.");
	}
#if defined(NTSHENGN_OS_WINDOWS)
	unsigned long nonBlockingSocket = 1;
	int nonBlocking = ioctlsocket(sock, FIONBIO, &nonBlockingSocket);
#elif defined(NTSHENGN_OS_LINUX)
	int nonBlocking = fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK);
#endif
	if (nonBlocking != 0) {
		NTSHENGN_NETWORKING_WARNING("[CLIENT / UDP] Error while making the client socket non-blocking.");
	}

	NTSHENGN_NETWORKING_INFO("[CLIENT / UDP] Created client socket.");

	ClientSocket clientSocket(sock, NetworkType::UDP);
	m_clients.push_front(clientSocket);

	return &m_clients.front();
}

NtshEngn::ClientSocket* NtshEngn::Networking::createClientSocketTCP() {
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		NTSHENGN_NETWORKING_WARNING("[CLIENT / TCP] Error creating socket for client.");
	}
#if defined(NTSHENGN_OS_WINDOWS)
	unsigned long nonBlockingSocket = 1;
	int nonBlocking = ioctlsocket(sock, FIONBIO, &nonBlockingSocket);
#elif defined(NTSHENGN_OS_LINUX)
	int nonBlocking = fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK);
#endif
	if (nonBlocking != 0) {
		NTSHENGN_NETWORKING_WARNING("[CLIENT / TCP] Error while making the client socket non-blocking.");
	}

	NTSHENGN_NETWORKING_INFO("[CLIENT / TCP] Created client socket.");

	ClientSocket clientSocket(sock, NetworkType::TCP);
	m_clients.push_front(clientSocket);

	return &m_clients.front();
}