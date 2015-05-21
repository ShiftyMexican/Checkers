#include "BoardPiece.h"

BoardPiece::BoardPiece(int id, float xPos, float zPos, bool isBlack, bool occupied)
{
	m_blockMatrix = new glm::mat4(
		1, 0, 0, 1,
		0, 1, 0, 1,
		0, 0, 1, 1,
		0, 0, 0, 1);

	m_width = 10.0f;
	m_height = 10.0f;

	m_isBlack = isBlack;
	m_isOccupied = occupied;
	m_isPurple = false;
	m_isGreen = false;
	m_isSelected = false;
	m_isPossibleMove = false;
	m_isPurpleKing = false;
	m_isGreenKing = false;

	m_position = glm::vec3(xPos, 1, zPos);

	m_black = glm::vec4(0, 0, 0, 1);
	m_white = glm::vec4(1, 1, 1, 1);
	m_currentColour = glm::vec4(0, 0, 0, 1);

	m_id = id;

	if (m_isBlack == true)
		m_currentColour = m_black;
	else
		m_currentColour = m_white;

	if (m_isBlack == false)
	{
		if (m_id < 24 || m_id > 39)
			m_isOccupied = true;
	}

	if (m_isOccupied == true && m_id < 24)
		m_isGreen = true;
	else if (m_isOccupied == true && m_id > 39)
		m_isPurple = true;

	
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

	else if (m_isBlack == false)
	{
		m_currentColour = m_white;
	}
}