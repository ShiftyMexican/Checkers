/*
Author: Jarrod Dowsey
Description: Networking class to manage everything with my networking
*/

#ifndef	CLIENT_APPLCATION_H
#define CLIENT_APPLCATION_H

#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "Checkerboard.h"
#include "BoardPiece.h"

class ClientApplication
{
public:
	ClientApplication();
	~ClientApplication();

	void Update();

	// Initialize the connection
	void HandleNetworkConnection();
	void InitializeClientConnection();

	// Handle the incoming packets
	void HandleNetworkMessgaes(RakNet::RakPeerInterface* pPeerInterface);

	void ReadObjectDataFromServer(RakNet::BitStream& bsIn);

	void SendMoveToServer(BoardPiece* tempBoardPiece);

	void SetCheckerBoard(Checkerboard* board);

	unsigned int m_uiClientID;

private:

	RakNet::RakPeerInterface* m_peerInterface;
	Checkerboard* m_board;

	

	const char* IP = "127.0.0.1";
	const unsigned short PORT = 5456;

};
#endif