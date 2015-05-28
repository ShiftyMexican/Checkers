#include "NetworkManager.h"
#include <iostream>
#include <string>

#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"

NetworkManager::NetworkManager()
{
	m_client1 = new ClientApplication();
	m_peerInterface = nullptr;
}

NetworkManager::~NetworkManager()
{

}

void NetworkManager::Update()
{
	HandleNetworkMessages(m_peerInterface);
	m_client1->HandleNetworkMessgaes();
}

void NetworkManager::Run()
{
	const unsigned short PORT = 5456;

	// Start Up the server, and start it listening to clients
	std::cout << "Starting up the server" << std::endl;

	// Initialize the raknet peer interface first
	m_peerInterface = RakNet::RakPeerInterface::GetInstance();

	// Create a socket descriptor to descripe this connection 
	RakNet::SocketDescriptor sd(PORT, 0);

	// Now call startup - max of 32 connections, on the assigned port
	m_peerInterface->Startup(32, &sd, 1);
	m_peerInterface->SetMaximumIncomingConnections(32);
	m_client1->HandleNetworkConnection();

}

void NetworkManager::HandleNetworkMessages(RakNet::RakPeerInterface* pPeerInterface)
{
	RakNet::Packet* packet = nullptr;

	for (packet = pPeerInterface->Receive(); packet;
		pPeerInterface->DeallocatePacket(packet), packet = pPeerInterface->Receive())
	{
		switch (packet->data[0])
		{
		case ID_NEW_INCOMING_CONNECTION:
			std::cout << "A connection is incoming.\n" << std::endl;
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			std::cout << "A client has been disconnected. \n" << std::endl;
			break;
		case ID_CONNECTION_LOST:
			std::cout << "A client has lost the connection. \n" << std::endl;
			break;
		default:
			std::cout << "Received a message with an unknown ID. \n" << packet->data[0];
			break;
		}
	}

}