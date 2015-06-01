/*
	Author: Jarrod Dowsey
	Description: Networking class to manage everything with my networking
*/

#pragma once

#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "ClientApplication.h"
#include <vector>
#include <unordered_map>
#include "BoardPiece.h"

class NetworkManager
{
public:
	NetworkManager();
	~NetworkManager();

	void Run();

	void Update();

	void HandleNetworkMessages(RakNet::RakPeerInterface* pPeerInterface);

	enum NetworkIDs
	{
		ID_SERVER_CLIENT_ID = ID_USER_PACKET_ENUM + 1,
		ID_CLIENT_CREATE_OBJECT = ID_USER_PACKET_ENUM + 2,
		ID_SERVER_FULL_OBJECT_DATA = ID_USER_PACKET_ENUM + 3,
	};

	unsigned int SystemAddressToClientID(RakNet::SystemAddress& systemAddress);

	//Connection functions
	void AddNewConnection(RakNet::SystemAddress systemAddress);
	void RemoveConnection(RakNet::SystemAddress systemAddress);

	void SendClientIDToClient(unsigned int uiClientID);

	void CreateObject(RakNet::BitStream& bsIn, RakNet::SystemAddress& ownerSystemAdress);

	void SendObjectToAllClients(BoardPiece& boardPiece, RakNet::SystemAddress systemAddress);

private:
	struct ConnectionInfo
	{
		unsigned int uiConnectionID;
		RakNet::SystemAddress sysAddress;
	};

	ClientApplication* m_client1;
	RakNet::RakPeerInterface* m_peerInterface;

	unsigned int m_uiConnectionCounter;
	std::unordered_map<unsigned int, ConnectionInfo> m_connectedClients;
	std::vector<BoardPiece* > m_boardpieces;
	unsigned int m_uiObjectCounter;
	

};