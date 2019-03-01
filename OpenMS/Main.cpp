#include "Common/Common.hpp"
#include <string>
#include "Network/Socket.hpp"

using namespace OpenMS;

int main()
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

		for(auto& c : response)
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

		for(auto& c : response)
			send.push_back(c);
		sock.write(send);
	}

	return 0;
}
