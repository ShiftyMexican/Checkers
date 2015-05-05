#include "BoardPiece.h"

BoardPiece::BoardPiece()
{
	m_blockMatrix = new glm::mat4(
		1, 0, 0, 1,
		0, 1, 0, 1,
		0, 0, 1, 1,
		0, 0, 0, 1);

	m_width = 10.0f;
	m_height = 10.0f;

	m_isBlack = true;
	m_isOccupied = false;
	m_isWhite = false;
}

BoardPiece::~BoardPiece()
{

}