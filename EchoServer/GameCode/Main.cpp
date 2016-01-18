//=====================================================
// Main.cpp
// by Andrew Socha
//=====================================================

#include "Engine/Networking/NetworkSystem.hpp"
#include "Engine/Console/Console.hpp"
#include "Engine/Core/Utilities.hpp"

int main(int argc, const char** args) {
	NetworkSystem netSystem;

	if (!netSystem.Init()) {
		return 1;
	}

	std::string hostName = netSystem.AllocLocalHostName();
	ConsolePrintf("\n%s\n\n", hostName.c_str());

	netSystem.PrintAddressesForHost(hostName, "1234");

	if (argc <= 1) {
		ConsolePrintf("Error: No arguments specified.\n");
		return 1;
	}

	if (strcmp(args[1], "server") == 0) { //host
		int numConnections = 8;
		if (argc > 2) {
			GetInt(args[2], numConnections);
		}

		netSystem.StartHost(hostName, "1234", numConnections);
	}
	else { //client
		const char* hostNameArg = args[1];
		std::string identifier = "client1234";
		if (argc > 2) {
			identifier = args[2];
		}

		netSystem.StartClient(hostNameArg, "1234", identifier);
	}

	netSystem.Deinit();

	return 0;
}