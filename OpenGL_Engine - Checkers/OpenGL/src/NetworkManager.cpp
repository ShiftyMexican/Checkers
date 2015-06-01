#include "NetworkManager.h"
#include <iostream>
#include <string>

#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "BoardPiece.h"

NetworkManager::NetworkManager()
{
	m_client1 = new ClientApplication();
	m_uiConnectionCounter = 1;
}

NetworkManager::~NetworkManager()
{

}

void NetworkManager::Update()
{
	HandleNetworkMessages(m_peerInterface);
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
		{
			//SystemAddressToClientID(packet->systemAddress);
			AddNewConnection(packet->systemAddress);
			std::cout << "A connection is incoming.\n" << std::endl;
		} break;

		case ID_DISCONNECTION_NOTIFICATION:
		{
			RemoveConnection(packet->systemAddress);
			std::cout << "A client has been disconnected. \n" << std::endl;
		} break;

		case ID_CONNECTION_LOST:
		{
			RemoveConnection(packet->systemAddress);
			std::cout << "A client has lost the connection. \n" << std::endl;
		} break;

		case ID_CLIENT_CREATE_OBJECT:
		{
			RakNet::BitStream bsIn(packet->data, packet->length, false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			CreateObject(bsIn, packet->systemAddress);
		} break;
			
		default:
			std::cout << "Received a message with an unknown ID. \n" << packet->data[0];
			break;
		}
	}

}

void NetworkManager::SendClientIDToClient(unsigned int uiClientID)
{
	RakNet::BitStream bs;
	bs.Write((RakNet::MessageID)NetworkIDs::ID_SERVER_CLIENT_ID);
	bs.Write(uiClientID);

	m_peerInterface->Send(&bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, m_connectedClients[uiClientID].sysAddress, false);
}

void NetworkManager::AddNewConnection(RakNet::SystemAddress systemAddress)
{
	ConnectionInfo info;
	info.sysAddress = systemAddress;
	info.uiConnectionID = m_uiConnectionCounter++;
	m_connectedClients[info.uiConnectionID] = info;

	SendClientIDToClient(info.uiConnectionID);
}

void NetworkManager::RemoveConnection(RakNet::SystemAddress systemAddress)
{
	for (auto it = m_connectedClients.begin(); it != m_connectedClients.end(); it++)
	{
		if (it->second.sysAddress == systemAddress)
		{
			m_connectedClients.erase(it);
			break;
		}
	}
}

unsigned int NetworkManager::SystemAddressToClientID(RakNet::SystemAddress& systemAddress)
{
	for (auto it = m_connectedClients.begin(); it != m_connectedClients.end(); it++)
	{
		if (it->second.sysAddress == systemAddress)
		{
			return it->first;
		}
	}

	return 0;
}

void NetworkManager::CreateObject(RakNet::BitStream& bsIn, RakNet::SystemAddress& ownerSystemAdress)
{
	BoardPiece* newBoardPiece;

	bsIn.Read(newBoardPiece->m_id);
	bsIn.Read(newBoardPiece->m_isGreen);
	bsIn.Read(newBoardPiece->m_isGreenKing);
	bsIn.Read(newBoardPiece->m_isPurple);
	bsIn.Read(newBoardPiece->m_isPurpleKing);
	bsIn.Read(newBoardPiece->m_isOccupied);
	
	newBoardPiece->uiOwnerClientID = SystemAddressToClientID(ownerSystemAdress);
	newBoardPiece->uiObjectID = m_uiObjectCounter++;
	
}

void NetworkManager::SendObjectToAllClients(BoardPiece& boardPiece, RakNet::SystemAddress systemAddress)
{
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)ID_SERVER_FULL_OBJECT_DATA);
	bsOut.Write(boardPiece.m_id);
	bsOut.Write(boardPiece.m_isGreen);
	bsOut.Write(boardPiece.m_isGreenKing);
	bsOut.Write(boardPiece.m_isPurple);
	bsOut.Write(boardPiece.m_isPurpleKing);
	bsOut.Write(boardPiece.m_isOccupied);

	m_peerInterface->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, systemAddress, true);
}
