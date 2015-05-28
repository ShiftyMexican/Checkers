/*
	Author: Jarrod Dowsey
	Description: Networking class to manage everything with my networking
*/

#ifndef	NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "ClientApplication.h"

class NetworkManager
{
public:
	NetworkManager();
	~NetworkManager();

	void Run();

	void Update();

	void HandleNetworkMessages(RakNet::RakPeerInterface* pPeerInterface);

private:
	ClientApplication* m_client1;
	RakNet::RakPeerInterface* m_peerInterface;

};
#endif