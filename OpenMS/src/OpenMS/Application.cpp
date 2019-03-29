/*//////////////////////////////////////////////////////////////////////////////

MIT License

Copyright (c) 2019 OpenMaplestory

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sub license, and/or sell
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

Created: 29/03/2019 23:40

//////////////////////////////////////////////////////////////////////////////*/

#include "omspch.h"
#include "Core.hpp"
#include "Application.hpp"

#include "OpenMS/Common/Common.hpp"
#include "OpenMs/Network/Socket.hpp"

namespace OpenMS {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		OMS_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		{ // Server
			Network::Socket sock;
			sock.bind("0.0.0.0", 1337);
			sock.listen(1);
			Network::Socket::ClientInfo client = sock.accept();
			Buffer received = client.socket.read(1024);
			std::string name = std::string(reinterpret_cast<const char * const>(received.data()));
			std::string response = "Hello " + name;
			Buffer send;

			for (auto& c : response)
				send.push_back(c);
			client.socket.write(send);
		}

		system("pause");

		{ // Client
			Network::Socket sock;
			sock.connect("localhost", 1337);
			Buffer received = sock.read(1024);
			std::string name = std::string(reinterpret_cast<const char * const>(received.data()));
			std::string response = "Hello " + name;
			Buffer send;

			for (auto& c : response)
				send.push_back(c);
			sock.write(send);
		}
	}
}