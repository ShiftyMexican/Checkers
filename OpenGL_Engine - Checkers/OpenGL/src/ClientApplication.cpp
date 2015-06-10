#include "ClientApplication.h"
#include "NetworkManager.h"
#include <iostream>
#include <string>

ClientApplication::ClientApplication() 
{
	m_yourTurn = true;
	m_yourTurn2 = false;

}

ClientApplication::~ClientApplication()
{
}

void ClientApplication::Update()
{
	HandleNetworkMessgaes(m_peerInterface);

	if (m_uiClientID > 0)
	{
		if (m_uiClientID == 1)
		{
			if (m_board->m_greenTurn == true)
			{
				m_yourTurn = true;
				m_yourTurn2 = false;
			}
		}

		if (m_uiClientID == 2)
		{
			if (m_board->m_greenTurn == false)
			{
				m_yourTurn2 = true;
				m_yourTurn = false;
			}
			
		}

	}
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
				bsIn.Read(m_uiClientID);

				std::cout << "The Server has given us the id of:"<< m_uiClientID << std::endl;

			} break;

		case NetworkManager::ID_SERVER_FULL_OBJECT_DATA:
		{
			RakNet::BitStream bsIn(packet->data, packet->length, false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			ReadObjectDataFromServer(bsIn);
		} break;

		default:
			std::cout << "Received a message with an unknown ID. \n" << packet->data[0] << std::endl;
			break;
		}
	}
}

void ClientApplication::ReadObjectDataFromServer(RakNet::BitStream& bsIn)
{
	BoardPiece newBoardPiece(0, 1, 1, false, false);

	bsIn.Read(newBoardPiece.m_id);
	bsIn.Read(newBoardPiece.m_isGreen);
	bsIn.Read(newBoardPiece.m_isGreenKing);
	bsIn.Read(newBoardPiece.m_isPurple);
	bsIn.Read(newBoardPiece.m_isPurpleKing);
	bsIn.Read(newBoardPiece.m_isOccupied);
	bsIn.Read(m_board->m_greenTurn);
	bsIn.Read(newBoardPiece.uiObjectID);
	bsIn.Read(newBoardPiece.uiOwnerClientID);
	

	bool bFound = false;

	for (int i = 0; i < m_board->m_boardpieces.size(); i++)
	{
		if (m_board->m_boardpieces[i]->m_id == newBoardPiece.m_id)
		{
			bFound = true;

			BoardPiece* obj = m_board->m_boardpieces[i];
			obj->m_id = newBoardPiece.m_id;
			obj->m_isGreen = newBoardPiece.m_isGreen;
			obj->m_isGreenKing = newBoardPiece.m_isGreenKing;
			obj->m_isPurple = newBoardPiece.m_isPurple;
			obj->m_isPurpleKing = newBoardPiece.m_isPurpleKing;
			obj->m_isOccupied = newBoardPiece.m_isOccupied;
			obj->uiOwnerClientID = newBoardPiece.uiOwnerClientID;
		}
	}

	//if (!bFound)
	//{
	//	m_board->m_boardpieces.push_back(newBoardPiece);
	//	if (newBoardPiece->uiOwnerClientID == m_uiClientID)
	//	{
	//		m_uiClient
	//	}
	//}
}

void ClientApplication::SendMoveToServer(BoardPiece* tempBoardPiece)
{
	RakNet::BitStream bsOut;

	//BoardPiece* tempBoardPiece = new BoardPiece(0, 1, 1, false, false);

	//tempBoardPiece->m_id = 0;
	//tempBoardPiece->m_isGreen = false;
	//tempBoardPiece->m_isGreenKing = false;
	//tempBoardPiece->m_isPurple = false;
	//tempBoardPiece->m_isPurpleKing = false;
	//tempBoardPiece->m_isOccupied = false;
	//tempBoardPiece->uiObjectID = 0;
	//tempBoardPiece->uiOwnerClientID = 0;

	bsOut.Write((RakNet::MessageID)NetworkManager::ID_CLIENT_CREATE_OBJECT);
	bsOut.Write(tempBoardPiece->m_id);
	bsOut.Write(tempBoardPiece->m_isGreen);
	bsOut.Write(tempBoardPiece->m_isGreenKing);
	bsOut.Write(tempBoardPiece->m_isPurple);
	bsOut.Write(tempBoardPiece->m_isPurpleKing);
	bsOut.Write(tempBoardPiece->m_isOccupied);
	bsOut.Write(m_board->m_greenTurn);
	bsOut.Write(tempBoardPiece->uiObjectID);
	bsOut.Write(tempBoardPiece->uiOwnerClientID);
	

	m_peerInterface->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void ClientApplication::SetCheckerBoard(Checkerboard* board)
{
	m_board = board;
}
