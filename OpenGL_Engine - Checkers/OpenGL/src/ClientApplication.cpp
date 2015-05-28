#include "ClientApplication.h"
#include <iostream>
#include <string>

ClientApplication::ClientApplication()
{
}

ClientApplication::~ClientApplication()
{
}

// Initialize the connection
void ClientApplication::HandleNetworkConnection()
{
	// Initialize the Raknet peer interface first
	m_peerInterface = RakNet::RakPeerInterface::GetInstance();
	InitializeClientConnection();
}

void ClientApplication::InitializeClientConnection()
{
	RakNet::SocketDescriptor sd;

	m_peerInterface->Startup(1, &sd, 1);

	std::cout << "Connecting to serverat: " << IP << std::endl;

	RakNet::ConnectionAttemptResult res = m_peerInterface->Connect(IP, PORT, nullptr, 0);

	if (res != RakNet::CONNECTION_ATTEMPT_STARTED)
	{
		std::cout << "Unable to start connection, Error Number: " << res << std::endl;
	}

}

// Handle the incoming packets
void ClientApplication::HandleNetworkMessgaes()
{
	RakNet::Packet* packet;

	for (packet = m_peerInterface->Receive(); packet;
		m_peerInterface->DeallocatePacket(packet), packet = m_peerInterface->Receive())
	{
		switch (packet->data[0])
		{
		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			std::cout << "Another client has disconnected.\n" << std::endl;
			break;

		case ID_REMOTE_CONNECTION_LOST:
			std::cout << "Another client has lost the connection.\n" << std::endl;
			break;

		case ID_NEW_INCOMING_CONNECTION:
			std::cout << "A connection is incoming.\n";
			break;

		case ID_CONNECTION_REQUEST_ACCEPTED:
			std::cout << "Our connection request has been accepted.\n" << std::endl;
			break;

		case ID_NO_FREE_INCOMING_CONNECTIONS:
			std::cout << "The server is full.\n" << std::endl;
			break;

		case ID_DISCONNECTION_NOTIFICATION:
			std::cout << "A client has been disconnected. \n" << std::endl;
			break;

		case ID_CONNECTION_LOST:
			std::cout << "A client has lost the connection. \n" << std::endl;
			break;
		default:
			std::cout << "Received a message with an unknown ID. \n" << packet->data[0] << std::endl;
			break;
		}
	}
}
