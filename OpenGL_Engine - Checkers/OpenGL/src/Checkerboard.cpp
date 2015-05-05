#include "Checkerboard.h"
#include "BoardPiece.h"

Checkerboard::Checkerboard()
{
	m_xOffset = 100;

	//m_boardpiece = new BoardPiece[BOARD_WIDTH * BOARD_WIDTH];

	m_black = glm::vec4(0, 0, 0, 1);
	m_white = glm::vec4(1, 1, 1, 1);

	m_currentColour = glm::vec4(0, 0, 0, 1);

	m_iterations = 0;

	m_boardPieceCenter = glm::vec3(0);

	m_pieceID = 0;

	m_isBlack = true;

	CreateCheckersBoard();
}

Checkerboard::~Checkerboard()
{

}

void Checkerboard::Update()
{

}

void Checkerboard::Draw()
{
	for (auto itr = m_boardpieces.begin(); itr != m_boardpieces.end(); itr++)
	{
		Gizmos::addAABBFilled((*itr)->m_position, glm::vec3((*itr)->m_width, 2, (*itr)->m_height), (*itr)->m_currentColour, nullptr);
	}
}

void Checkerboard::CreateCheckersBoard()
{
	//for (int x = 0; x < BOARD_WIDTH; x++)
	//{
	//	for (int y = 0; y < BOARD_WIDTH; y++)
	//	{
	//		unsigned int uiIndex = y * (BOARD_WIDTH)+x;
	//
	//		ColourSwitch();
	//
	//		// calculates the Centre point of each checker piece
	//		m_boardPieceCenter = glm::vec3((0.5 * m_boardpiece[uiIndex].m_width) + (x * m_boardpiece[uiIndex].m_width * 2), 1, (0.5 * m_boardpiece[uiIndex].m_height) + (y * m_boardpiece[uiIndex].m_height * 2));
	//
	//		Gizmos::addAABBFilled(m_boardPieceCenter, glm::vec3(m_boardpiece[uiIndex].m_width, 2, m_boardpiece[uiIndex].m_height),
	//			m_currentColour, nullptr);
	//
	//		m_iterations += 1;
	//	}
	//}

		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			for (int y = 0; y < BOARD_WIDTH; y++)
			{

				float xPos = (0.5 * 10 * 2) + (x * 10 * 2);
				float yPos = (0.5 * 10 * 2) + (y * 10 * 2);

				AddBoardPiece(m_pieceID, xPos, yPos, m_isBlack);
				m_pieceID += 1;

				ColourSwitch();
			}
		}
}

void Checkerboard::ColourSwitch()
{
	if (m_isBlack == true && m_iterations < 8)
		m_isBlack = false;

	else if (m_isBlack == false && m_iterations < 8)
		m_isBlack = true;

	else if (m_iterations == 8)
	{
		m_isBlack = m_isBlack;
		m_iterations = 0;
	}

	m_iterations += 1;
}

void Checkerboard::AddBoardPiece(int id, float xPos, float zPos, bool colour)
{
	BoardPiece* boardpiece = new BoardPiece(id, xPos, zPos, colour);
	m_boardpieces.push_back(boardpiece);
	boardpiece->ColourSwitch();
}
