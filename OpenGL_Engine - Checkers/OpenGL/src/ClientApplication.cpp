#include "ClientApplication.h"
#include "NetworkManager.h"
#include <iostream>
#include <string>

ClientApplication::ClientApplication()
{
}

ClientApplication::~ClientApplication()
{
}

void ClientApplication::Update()
{
	HandleNetworkMessgaes(m_peerInterface);
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

	std::cout << "Connecting to server at: " << IP << std::endl;

	RakNet::ConnectionAttemptResult res = m_peerInterface->Connect(IP, PORT, nullptr, 0);

	if (res != RakNet::CONNECTION_ATTEMPT_STARTED)
	{
		std::cout << "Unable to start connection, Error Number: " << res << std::endl;
	}

}

// Handle the incoming packets
void ClientApplication::HandleNetworkMessgaes(RakNet::RakPeerInterface* pPeerInterface)
{
	RakNet::Packet* packet = nullptr;

	for (packet = pPeerInterface->Receive(); packet;
		pPeerInterface->DeallocatePacket(packet), packet = pPeerInterface->Receive())
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

		case NetworkManager::ID_SERVER_CLIENT_ID:
			{
				RakNet::BitStream bsIn(packet->data, packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				int m_uiClientID = bsIn.Read(m_uiClientID);

				std::cout << "The Server has given us the id of:"<< m_uiClientID << std::endl;
			} break;

		default:
			std::cout << "Received a message with an unknown ID. \n" << packet->data[0] << std::endl;
			break;
		}
	}
}
