#include "BoardPiece.h"

BoardPiece::BoardPiece(int id, float xPos, float zPos, bool isBlack)
{
	m_blockMatrix = new glm::mat4(
		1, 0, 0, 1,
		0, 1, 0, 1,
		0, 0, 1, 1,
		0, 0, 0, 1);

	m_width = 10.0f;
	m_height = 10.0f;

	m_isBlack = isBlack;
	m_isOccupied = false;

	m_position = glm::vec3(xPos, 1, zPos);

	m_black = glm::vec4(0, 0, 0, 1);
	m_white = glm::vec4(1, 1, 1, 1);
	m_currentColour = glm::vec4(0, 0, 0, 1);

	m_id = id;

	if (m_isBlack == true)
		m_currentColour = m_black;
	else
		m_currentColour = m_white;

}

BoardPiece::~BoardPiece()
{

}

void BoardPiece::Update()
{
	ColourSwitch();
}

void BoardPiece::ColourSwitch()
{
	if (m_isBlack == true)
	{
		m_currentColour = m_black;
	}

	else if (m_isWhite == true)
	{
		m_currentColour = m_white;
	}


}