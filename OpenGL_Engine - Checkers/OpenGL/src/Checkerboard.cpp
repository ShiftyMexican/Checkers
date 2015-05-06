#include "Checkerboard.h"
#include "BoardPiece.h"

Checkerboard::Checkerboard(GLFWwindow* window)
{
	m_xOffset = 100;

	m_black = glm::vec4(0, 0, 0, 1);
	m_white = glm::vec4(1, 1, 1, 1);

	m_currentColour = glm::vec4(0, 0, 0, 1);

	m_iterations = 0;

	m_boardPieceCenter = glm::vec3(0);

	m_pieceID = 0;

	m_isBlack = true;

	m_occupied = false;

	m_window = window;

	CreateCheckersBoard();
}

Checkerboard::~Checkerboard()
{

}

void Checkerboard::Update()
{
	if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		m_boardpieces[28]->m_isOccupied = true;
		m_boardpieces[28]->m_isPurple = true;
	}
}

void Checkerboard::Draw()
{
	for (auto itr = m_boardpieces.begin(); itr != m_boardpieces.end(); itr++)
	{
		Gizmos::addAABBFilled((*itr)->m_position, glm::vec3((*itr)->m_width, 2, (*itr)->m_height), (*itr)->m_currentColour, nullptr);

		if ((*itr)->m_isOccupied == true && (*itr)->m_isGreen == true)
		{
			Gizmos::addCylinderFilled(glm::vec3((*itr)->m_position.x, 5, (*itr)->m_position.z), 0.8 * (*itr)->m_width, 2, 20, glm::vec4(0, 0.9, 0.2, 1), nullptr);
		}

		if ((*itr)->m_isOccupied == true && (*itr)->m_isPurple == true)
		{
			Gizmos::addCylinderFilled(glm::vec3((*itr)->m_position.x, 5, (*itr)->m_position.z), 0.8 * (*itr)->m_width, 2, 20, glm::vec4(0.6, 0, 1, 1), nullptr);
		}
	}
}

void Checkerboard::CreateCheckersBoard()
{
	for (int x = 0; x < BOARD_WIDTH; x++)
	{
		for (int y = 0; y < BOARD_WIDTH; y++)
		{

			float xPos = (0.5 * 10 * 2) + (x * 10 * 2);
			float yPos = (0.5 * 10 * 2) + (y * 10 * 2);

			if (m_pieceID > 24 && m_pieceID < 39)
			{
				m_occupied = false;
			}

			AddBoardPiece(m_pieceID, xPos, yPos, m_isBlack, m_occupied);
			m_pieceID += 1;
			m_iterations += 1;

			ColourSwitch();
		}
	}
}

void Checkerboard::ColourSwitch()
{
	if (m_isBlack == true && m_iterations < 8)
	{
		m_isBlack = false;
		m_occupied = true;
	}
		

	else if (m_isBlack == false && m_iterations < 8)
	{
		m_isBlack = true;

		if (m_pieceID < 24 || m_pieceID > 39)
			m_occupied = false;
	}
		
	else if (m_iterations == 8)
	{
		m_isBlack = m_isBlack;
		m_iterations = 0;
	}
}

void Checkerboard::AddBoardPiece(int id, float xPos, float zPos, bool colour, bool occupied)
{
	BoardPiece* boardpiece = new BoardPiece(id, xPos, zPos, colour, occupied);
	m_boardpieces.push_back(boardpiece);
	boardpiece->ColourSwitch();
}
