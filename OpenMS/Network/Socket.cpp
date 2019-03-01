/*//////////////////////////////////////////////////////////////////////////////

MIT License

Copyright (c) 2019 OpenMaplestory

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Created: 22/02/2019 17:09

//////////////////////////////////////////////////////////////////////////////*/

#include "Common/Common.hpp"
#include "Network/Socket.hpp"
#include "Network/SocketException.hpp"
#include "Network/WsaException.hpp"

using namespace OpenMS;
using namespace OpenMS::Network;

Socket::Socket(ADDRESS_FAMILY ai_family, int ai_socktype, int ai_protocol): // Constructor
 	m_family(ai_family),
	m_socktype(ai_socktype),
	m_protocol(ai_protocol)
{
	if (ai_family != AF_INET)
		// Not supported
		throw Network::SocketException();

	initSock();
}

Socket::Socket(const Socket& base_socket, SOCKET os_socket): // Constructor
	m_family(base_socket.m_family),
	m_socktype(base_socket.m_socktype),
	m_protocol(base_socket.m_protocol),
	m_socket(os_socket)
{
	#ifdef _WIN32

	initWsa();
	
	#endif // _WIN32
}

Socket::Socket(Socket&& other) noexcept // Move constructor
{
	std::swap(m_family, other.m_family);
	std::swap(m_socktype, other.m_socktype);
	std::swap(m_protocol, other.m_protocol);
	std::swap(m_socket, other.m_socket);
}

Socket::~Socket() // Destructor
{
	cleanupSock();
}

Socket::ClientInfo::ClientInfo(std::string ip, uint16_t port, Socket socket):
	ip(ip),
	port(port),
	socket(std::move(socket))
{
}

#ifdef _WIN32

void Socket::initWsa()
{
	WSAData wsa_data = { 0 };

	if(WSAStartup(MAKEWORD(2, 2), &wsa_data))
		// initializing WSA 2.2 failed
		throw Network::WsaException();
}

#endif

void Socket::initSock()
{
	#ifdef _WIN32

	initWsa();

	#endif // _WIN32

	m_socket = socket(m_family, m_socktype, m_protocol);

	if (m_socket == INVALID_SOCKET)
    // Socket creation failed
		throw Network::SocketException();

	int enable_option = 1;
	int option = SO_REUSEADDR;

	#ifdef linux

	option |= SO_REUSEPORT;

	#endif // linux

	const char * optval = reinterpret_cast<const char *>(&enable_option);

	if(setsockopt(m_socket, SOL_SOCKET, option, optval, sizeof(enable_option)) == -1)
		// Failed setting reuse address option
		throw Network::SocketException();
}

void Socket::cleanupSock()
{
	#ifdef _WIN32

	if(m_socket != static_cast<SOCKET>(0))
	{
		closesocket(m_socket);

		if (WSACleanup())
			// Failed to clean up WSA, probably wasn't created or unexpected condition
			throw Network::WsaException();
	}


	#endif // _WIN32

	#ifdef linux

	if(m_socket != static_cast<SOCKET>(0))
		close(m_socket);

	#endif // linux
}

void Socket::bind(std::string ip, uint16_t port)
{
	sockaddr_in binding_data;
	binding_data.sin_port = htons(port);
	binding_data.sin_family = m_family;

	switch (inet_pton(AF_INET, resolveHostname(ip).c_str(), &binding_data.sin_addr.s_addr))
	{
	case -1:
		// Error
		throw Network::SocketException();
	case 0:
		// Invalid address
		throw Network::SocketException();
	}

	auto name = reinterpret_cast<sockaddr *>(&binding_data);
	if (::bind(m_socket, name, sizeof(*name)) == SOCKET_ERROR)
		// Failed binding address
		throw Network::SocketException();
}

void Socket::listen(int connections)
{
	if (::listen(m_socket, connections) == SOCKET_ERROR)
		// Failed listening for connections
		throw Network::SocketException();
}

Socket::ClientInfo Socket::accept()
{
	sockaddr_in client_address = { 0 };
	socklen_t address_length = sizeof(client_address);

	SOCKET client_socket = ::accept(m_socket, reinterpret_cast<sockaddr *>(&client_address), &address_length);
	if (client_socket == INVALID_SOCKET)
    	// Accepting client failed
		throw Network::SocketException();

	char ip_string[INET_ADDRSTRLEN] = { 0 };
	if(inet_ntop(client_address.sin_family, &client_address.sin_addr, ip_string, sizeof(ip_string)) == NULL)
		// Failed converting ip to string
		throw Network::SocketException();

	ClientInfo client(ip_string, ntohs(client_address.sin_port), Socket(*this, client_socket));
	return client;
}

void Socket::connect(std::string ip, uint16_t port)
{
	sockaddr_in connect_address;
	connect_address.sin_port = htons(port);
	connect_address.sin_family = m_family;

	switch(inet_pton(AF_INET, resolveHostname(ip).c_str(), &connect_address.sin_addr.s_addr))
	{
		case -1:
			// Error
			throw Network::SocketException();
		case 0:
			// Invalid address
			throw Network::SocketException();
	}

	auto name = reinterpret_cast<sockaddr *>(&connect_address);
	if(::connect(m_socket, name, sizeof(*name)) == SOCKET_ERROR)
		// Failed connecting address
		throw Network::SocketException();
}

Buffer Socket::readSocket(SocketBufferLength bytes_to_read)
{
	Buffer buffer(bytes_to_read);
	ssize_t bytes_read = static_cast<ssize_t>(recv(m_socket, reinterpret_cast<char *>(buffer.data()), bytes_to_read, 0));
	if (bytes_read == SOCKET_ERROR)
		// Error receiving data
		throw Network::SocketException();

	buffer.resize(bytes_read);

	return buffer;
}

Buffer Socket::read(size_t bytes_to_read)
{
	return readSocket(static_cast<SocketBufferLength>(bytes_to_read));
}

void Socket::writeSocket(Buffer buffer)
{
	if(send(m_socket, reinterpret_cast<const char *>(buffer.data()), static_cast<SocketBufferLength>(buffer.size()), 0) == SOCKET_ERROR)
		// Failed sending data
		throw Network::SocketException();
}

void Socket::write(Buffer buffer)
{
	writeSocket(buffer);
}

Socket& Socket::operator=(Socket&& other) noexcept // Move assignment operator
{
	std::swap(m_family, other.m_family);
	std::swap(m_socktype, other.m_socktype);
	std::swap(m_protocol, other.m_protocol);
	std::swap(m_socket, other.m_socket);

	return *this;
}

std::string Socket::resolveHostname(std::string hostname, int ai_family)
{
	addrinfo hints = { 0 };
	addrinfo* address_info = { 0 };
	addrinfo* current_address = { 0 };

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if(getaddrinfo(hostname.c_str(), NULL, &hints, &address_info) != 0)
		throw Network::SocketException();

	char ip_string[INET6_ADDRSTRLEN] = { 0 };
	for(current_address = address_info; current_address != NULL; current_address = current_address->ai_next)
	{
		void *address = nullptr;

		if(ai_family == AF_INET && current_address->ai_family == AF_INET)
			address = &(reinterpret_cast<sockaddr_in*>(current_address->ai_addr)->sin_addr);
		else if (ai_family == AF_INET6 && current_address->ai_family == AF_INET6)
			address = &(reinterpret_cast<sockaddr_in6*>(current_address->ai_addr)->sin6_addr);

		if(address != nullptr)
			if(inet_ntop(current_address->ai_family, address, ip_string, sizeof(ip_string)) != NULL)
				 break;
	}

	freeaddrinfo(address_info);

	return ip_string;
}
