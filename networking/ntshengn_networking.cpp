#include "ntshengn_networking.h"
#include "ntshengn_networking_server_socket.h"
#include "ntshengn_networking_client_socket.h"
#if defined(NTSHENGN_OS_WINDOWS)
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#elif defined(NTSHENGN_OS_LINUX)
#include <netinet/in.h>
#include <fcntl.h>
#define INVALID_SOCKET -1
#endif
#include <unordered_map>
#include <functional>
#include <forward_list>
#include <string>

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
	for (const std::unique_ptr<ServerSocketInterface>& server : m_servers) {
		server->update();
	}

	// Clients
	for (const std::unique_ptr<ClientSocketInterface>& client : m_clients) {
		client->update();
	}
}

void NtshEngn::Networking::destroy() {
	// Servers
	for (const std::unique_ptr<ServerSocketInterface>& server : m_servers) {
		server->destroy();
	}

	m_servers.clear();

	// Clients
	for (const std::unique_ptr<ClientSocketInterface>& client : m_clients) {
		client->destroy();
	}

	m_clients.clear();

#if defined(NTSHENGN_OS_WINDOWS)
	if (WSACleanup() != 0) {
		NTSHENGN_NETWORKING_WARNING("Could not cleanup WSA.");
	}
#endif
}

NtshEngn::ServerSocketInterface* NtshEngn::Networking::createServerSocket(uint16_t port, NetworkType networkType) {
	if (networkType == NetworkType::UDP) {
		return createServerSocketUDP(port);
	}
	else if (networkType == NetworkType::TCP) {
		return createServerSocketTCP(port);
	}

	return nullptr;
}

NtshEngn::ClientSocketInterface* NtshEngn::Networking::createClientSocket(NetworkType networkType) {
	if (networkType == NetworkType::UDP) {
		return createClientSocketUDP();
	}
	else if (networkType == NetworkType::TCP) {
		return createClientSocketTCP();
	}

	return nullptr;
}

void NtshEngn::Networking::closeServerSocket(ServerSocketInterface* serverSocket) {
	serverSocket->destroy();

	std::forward_list<std::unique_ptr<ServerSocketInterface>>::iterator prev = m_servers.before_begin();
	for (std::forward_list<std::unique_ptr<ServerSocketInterface>>::iterator it = m_servers.begin(); it != m_servers.end(); it++) {
		if (serverSocket == (*it).get()) {
			m_servers.erase_after(prev);
			return;
		}

		prev = it;
	}
}

void NtshEngn::Networking::closeClientSocket(ClientSocketInterface* clientSocket) {
	clientSocket->destroy();

	std::forward_list<std::unique_ptr<ClientSocketInterface>>::iterator prev = m_clients.before_begin();
	for (std::forward_list<std::unique_ptr<ClientSocketInterface>>::iterator it = m_clients.begin(); it != m_clients.end(); it++) {
		if (clientSocket == (*it).get()) {
			m_clients.erase_after(prev);
			return;
		}

		prev = it;
	}
}

NtshEngn::ServerSocketInterface* NtshEngn::Networking::createServerSocketUDP(uint16_t port) {
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

	m_servers.push_front(std::make_unique<ServerSocket>(static_cast<Socket>(sock), port, NetworkType::UDP));

	return m_servers.front().get();
}

NtshEngn::ServerSocketInterface* NtshEngn::Networking::createServerSocketTCP(uint16_t port) {
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

	m_servers.push_front(std::make_unique<ServerSocket>(static_cast<Socket>(sock), port, NetworkType::TCP));

	return m_servers.front().get();
}

NtshEngn::ClientSocketInterface* NtshEngn::Networking::createClientSocketUDP() {
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

	m_clients.push_front(std::make_unique<ClientSocket>(static_cast<Socket>(sock), NetworkType::UDP));

	return m_clients.front().get();
}

NtshEngn::ClientSocketInterface* NtshEngn::Networking::createClientSocketTCP() {
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

	m_clients.push_front(std::make_unique<ClientSocket>(static_cast<Socket>(sock), NetworkType::TCP));

	return m_clients.front().get();
}