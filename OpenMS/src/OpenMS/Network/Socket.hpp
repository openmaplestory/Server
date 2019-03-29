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

Created: 22/02/2019 19:34

//////////////////////////////////////////////////////////////////////////////*/
#pragma once

#include "OpenMS/Common/Common.hpp"
#include <string>
#include <memory>
#include "OpenMS/IO/IStream.hpp"

#ifdef _WIN32

#pragma comment(lib, "Ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>

using SocketBufferLength = int;

#endif // _WIN32

#ifdef linux

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define SOCKET_ERROR (-1)
#define INVALID_SOCKET (0)

using SocketBufferLength = size_t;

#endif // Linux

using namespace OpenMS;

namespace OpenMS
{
namespace Network
{

class Socket: public IO::IStream
{
public:
	struct ClientInfo;

	Socket(ADDRESS_FAMILY ai_family = AF_INET, int ai_socktype = SOCK_STREAM, int ai_protocol = IPPROTO_TCP); // Constructor
	
	Socket(const Socket& other) = delete; // Copy constructor
	Socket(Socket&& other) noexcept; // Move constructor

	~Socket(); // Destructor

	void bind(std::string ip, uint16_t port);

	void listen(int connections);

	ClientInfo accept();

	void connect(std::string ip, uint16_t port);

	virtual Buffer read(size_t bytes_to_read) override;
	virtual void write(Buffer buffer) override;

	Socket& operator=(const Socket& other) = delete; // Copy assignment operator
	Socket& operator=(Socket&& other) noexcept; // Move assignment operator

protected:
	Socket(const Socket& base_socket, SOCKET os_socket); // Constructor

	#ifdef _WIN32

	void initWsa();

	#endif // _WIN32

	void initSock();
	void cleanupSock();

	virtual Buffer readSocket(SocketBufferLength bytes_to_read);
	virtual void writeSocket(Buffer buffer);

	static std::string resolveHostname(std::string hostname, int ai_family = AF_INET);

	ADDRESS_FAMILY m_family = 0;
	int m_socktype = 0;
	int m_protocol = 0;

	SOCKET m_socket = static_cast<SOCKET>(0);
};

struct Socket::ClientInfo
{
	std::string ip;
	uint16_t port;
	Socket socket;

	ClientInfo(std::string ip, uint16_t port, Socket socket); // Constructor
};

}
}
