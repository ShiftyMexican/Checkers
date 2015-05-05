#include "Checkerboard.h"
#include "BoardPiece.h"

Checkerboard::Checkerboard()
{
	m_xOffset = 100;

	m_boardpiece = new BoardPiece[BOARD_WIDTH * BOARD_WIDTH];

	m_black = glm::vec4(0, 0, 0, 1);
	m_white = glm::vec4(1, 1, 1, 1);

	m_currentColour = glm::vec4(0, 0, 0, 1);

	m_iterations = 0;

	m_boardPieceCenter = glm::vec3(0);


}

Checkerboard::~Checkerboard()
{

}

void Checkerboard::Update()
{

}

void Checkerboard::Draw()
{
	CreateCheckersBoard();
}

void Checkerboard::CreateCheckersBoard()
{
	for (int x = 0; x < BOARD_WIDTH; x++)
	{
		for (int y = 0; y < BOARD_WIDTH; y++)
		{
			unsigned int uiIndex = y * (BOARD_WIDTH)+x;

			ColourSwitch();

			// calculates the Centre point of each checker piece
			m_boardPieceCenter = glm::vec3((0.5 * m_boardpiece[uiIndex].m_width) + (x * m_boardpiece[uiIndex].m_width * 2), 1, (0.5 * m_boardpiece[uiIndex].m_height) + (y * m_boardpiece[uiIndex].m_height * 2));

			Gizmos::addAABBFilled(m_boardPieceCenter, glm::vec3(m_boardpiece[uiIndex].m_width, 2, m_boardpiece[uiIndex].m_height),
				m_currentColour, nullptr);

			m_iterations += 1;
		}
	}

	for (int x = 0; x < BOARD_WIDTH; x++)
	{
		for (int y = 0; y < BOARD_WIDTH; y++)
		{
			unsigned int uiIndex = y * (BOARD_WIDTH)+x;

			// calculates the Centre point of each checker piece
			m_boardPieceCenter = glm::vec3((0.5 * m_boardpiece[uiIndex].m_width) + (x * m_boardpiece[uiIndex].m_width * 2), 1, (0.5 * m_boardpiece[uiIndex].m_height) + (y * m_boardpiece[uiIndex].m_height * 2));

			if (m_boardpiece[uiIndex].m_isOccupied == true)
			{
				Gizmos::addAABBFilled(glm::vec3(m_boardPieceCenter.x, 10, m_boardPieceCenter.z), glm::vec3(m_boardpiece[uiIndex].m_width, 2, m_boardpiece[uiIndex].m_height),
					m_currentColour, nullptr);
			}
		}
	}
}

void Checkerboard::ColourSwitch()
{
	if (m_boardpiece->m_isBlack == true && m_iterations < 8)
	{
		m_currentColour = m_black;
		m_boardpiece->m_isBlack = false;
		m_boardpiece->m_isWhite = true;
		m_boardpiece->m_isOccupied = true;
	}

	else if (m_boardpiece->m_isWhite == true && m_iterations < 8)
	{
		m_currentColour = m_white;
		m_boardpiece->m_isBlack = true;
		m_boardpiece->m_isWhite = false;
		m_boardpiece->m_isOccupied = false;

	}

	else if (m_iterations == 8)
	{
		m_currentColour = m_currentColour;
		m_iterations = 0;
	}
}
