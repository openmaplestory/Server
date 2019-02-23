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

#pragma region Windows
#ifdef _WIN32

#pragma comment(lib, "Ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#include "WsaException.hpp"

#endif // _WIN32
#pragma endregion

#pragma region Linux
#ifdef linux

// TODO: Linux sockets

#endif // linux
#pragma endregion

using namespace OpenMS;

namespace OpenMS
{
namespace Network
{

class Socket
{
public:
    Socket() // Constructor
    {
		#pragma region Windows
		#ifdef _WIN32

		initWsa();
		
		#endif // _WIN32
		#pragma endregion

		#pragma region Linux
		#ifdef linux

		// TODO: Linux sockets
		
		#endif // linux
		#pragma endregion
    }

    Socket(const Socket& other) = delete; // Copy constructor
	Socket(Socket&& other) noexcept // Move constructor
	{
		UNREFERENCED_PARAMETER(other);
	}

	~Socket() // Destructor
	{

	}

    Socket& operator=(const Socket& other) = delete; // Copy assignment operator
	Socket& operator=(Socket&& other) noexcept // Move assignment operator
	{
		UNREFERENCED_PARAMETER(other);
	}

private:
	#pragma region Windows
	#ifdef _WIN32

    void initWsa()
    {
        WSAData wsa_data;

        if (WSAStartup(MAKEWORD(2,2), &wsa_data))
            throw Network::WsaException();
    }

	#endif // _WIN32
	#pragma endregion

	#pragma region Linux
	#ifdef linux

	// TODO: Linux sockets
	
	#endif // linux
	#pragma endregion
};

}
}

/*
#ifdef _WIN32
    // WINDOWS SOCKET IMPLEMENTATION
    #include <WinSock2.h>
    #include <WS2tcpip.h>
    #include <iostream>
    #include <string>
    #include <exception>

    #pragma comment(lib, "Ws2_32.lib")

    class WsaStartupException : public std::exception
    {
        public:
            explicit WsaStartupException(const char* message)
            :   m_msg(message)
                {
                }
            
            explicit WsaStartupException(const std::string& message)
            :   m_msg(message)
                {
                }
            
            virtual ~WsaStartupException() throw (){}

            virtual const char* what() const throw ()
            {
                return m_msg.c_str();
            }
        
        protected:
            std::string m_msg;
    };

    class Socket
    {
        public:
            Socket(PCSTR port)
            {
                struct addrinfo *result = NULL;
                
            }
            ~Socket() = default;


            static const int initWsa()
            {
                WSAData wsadata;
                int initresult;

                // Initialize Winsock
                initresult = WSAStartup(MAKEWORD(2,2), &wsadata);
                if (initresult)
                    throw WsaStartupException("WSA Startup failed" + std::to_string(initresult));

                return initresult;
            }

        private:
            PCSTR m_port;
    };
#endif

#ifdef linux
    // LINUX SOCKET IMPLEMENTATION
#endif*/