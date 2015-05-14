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

	m_clicked = false;

	m_window = window;

	m_selectedPieceID = NULL;

	CreateCheckersBoard();
}

Checkerboard::~Checkerboard()
{

}

void Checkerboard::Update(glm::vec3 position)
{
	for (auto itr = m_boardpieces.begin(); itr != m_boardpieces.end(); itr++)
	{
		if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && m_clicked != true &&
			position.x <= (*itr)->m_position.x + 10 && position.x >= (*itr)->m_position.x - 10 && 
			position.z <= (*itr)->m_position.z + 10 && position.z >= (*itr)->m_position.z - 10)
		{
			// Check to see if the piece you have clicked has a green checker on it
			if ((*itr)->m_isOccupied == true && (*itr)->m_isSelected == false && (*itr)->m_isGreen == true && m_selectedPieceID == NULL)
			{
				// Sets the possible moves
				SetGreenPossibleMoves((*itr));
				m_selectedPieceID = (*itr)->m_id;
			}

			// Check to see if the piece you have clicked has a purple checker on it
			else if ((*itr)->m_isOccupied == true && (*itr)->m_isSelected == false && (*itr)->m_isPurple == true && m_selectedPieceID == NULL)
			{
				// Sets the possible moves
				SetPurplePossibleMoves((*itr));
				m_selectedPieceID = (*itr)->m_id;
			}

			// If piece is clicked again deselect it
			else if ((*itr)->m_isOccupied == true && (*itr)->m_isSelected == true && (*itr)->m_id == m_selectedPieceID)
			{
				ResetPossible((*itr));
			}
			
			// If youve clicked the possible move, move the piece
			else if ((*itr)->m_isPossibleMove == true)
			{
				(*itr)->m_isOccupied = true;
				(*itr)->m_isGreen = m_boardpieces[m_selectedPieceID]->m_isGreen;
				(*itr)->m_isPurple = m_boardpieces[m_selectedPieceID]->m_isPurple;
				m_boardpieces[m_selectedPieceID]->m_isSelected = false;
				m_boardpieces[m_selectedPieceID]->m_isOccupied = false;

				m_selectedPieceID = NULL;
				
				for (auto iter = m_boardpieces.begin(); iter != m_boardpieces.end(); iter++)
				{
					if ((*iter)->m_isPossibleMove == true)
						(*iter)->m_isPossibleMove = false;
				}
			}
			else
				break;
		}
	}
	
	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		m_clicked = true;
	else
		m_clicked = false;

}

void Checkerboard::Draw()
{
	for (auto itr = m_boardpieces.begin(); itr != m_boardpieces.end(); itr++)
	{
		if ((*itr)->m_isSelected == false && (*itr)->m_isPossibleMove == false)
		{
			Gizmos::addAABBFilled((*itr)->m_position, glm::vec3((*itr)->m_width, 2, (*itr)->m_height), (*itr)->m_currentColour, nullptr);
		}

		else if ((*itr)->m_isSelected == true)
		{
			Gizmos::addAABBFilled((*itr)->m_position, glm::vec3((*itr)->m_width, 2, (*itr)->m_height), glm::vec4(1, 0, 0, 1) , nullptr);
		}

		if ((*itr)->m_isOccupied == true && (*itr)->m_isGreen == true)
		{
			Gizmos::addCylinderFilled(glm::vec3((*itr)->m_position.x, 5, (*itr)->m_position.z), 0.8 * (*itr)->m_width, 2, 20, glm::vec4(0, 0.9, 0.2, 1), nullptr);
		}

		if ((*itr)->m_isOccupied == true && (*itr)->m_isPurple == true)
		{
			Gizmos::addCylinderFilled(glm::vec3((*itr)->m_position.x, 5, (*itr)->m_position.z), 0.8 * (*itr)->m_width, 2, 20, glm::vec4(0.6, 0, 1, 1), nullptr);
		}

		if ((*itr)->m_isPossibleMove == true)
		{
			Gizmos::addAABBFilled((*itr)->m_position, glm::vec3((*itr)->m_width, 2, (*itr)->m_height), glm::vec4(1, 0.35, 0, 1), nullptr);
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
		//m_occupied = true;
	}
		

	else if (m_isBlack == false && m_iterations < 8)
	{
		m_isBlack = true;
		//m_occupied = true;

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

void Checkerboard::SetGreenPossibleMoves(BoardPiece* itr)
{
	itr->m_isSelected = true;

	if ((itr->m_id + 7) <= m_boardpieces.size())
	{
		// Sets the Possible moves of the piece selected ------------------------------------------------------------------------
		if (m_boardpieces[(itr->m_id + 7)]->m_isBlack == false && m_boardpieces[(itr->m_id + 7)]->m_isOccupied == false)
			m_boardpieces[(itr->m_id + 7)]->m_isPossibleMove = true;

		if ((itr->m_id + 14) <= m_boardpieces.size())
		{
			if (m_boardpieces[(itr->m_id + 7)]->m_isOccupied == true && m_boardpieces[(itr->m_id + 7)]->m_isPurple == true)
			{
				if (m_boardpieces[(itr->m_id + 14)]->m_isOccupied == false)
				m_boardpieces[(itr->m_id + 14)]->m_isPossibleMove = true;
			}
		}
	}


	if ((itr->m_id + 9) <= m_boardpieces.size())
	{
		// Sets the Possible moves of the piece selected ------------------------------------------------------------------------
		if (m_boardpieces[(itr->m_id + 9)]->m_isBlack == false && m_boardpieces[(itr->m_id + 9)]->m_isOccupied == false)
			m_boardpieces[(itr->m_id + 9)]->m_isPossibleMove = true;

		if ((itr->m_id + 18) <= m_boardpieces.size())
		{
			if (m_boardpieces[(itr->m_id + 9)]->m_isOccupied == true && m_boardpieces[(itr->m_id + 9)]->m_isPurple == true)
			{
				if (m_boardpieces[(itr->m_id + 18)]->m_isOccupied == false)
					m_boardpieces[(itr->m_id + 18)]->m_isPossibleMove = true;
			}
		}

	}

}

void Checkerboard::SetPurplePossibleMoves(BoardPiece* itr)
{
	itr->m_isSelected = true;
	if ((itr->m_id - 7) >= 0)
	{
		// Sets the Possible moves of the piece selected ------------------------------------------------------------------------
		if (m_boardpieces[(itr->m_id - 7)]->m_isBlack == false && m_boardpieces[(itr->m_id - 7)]->m_isOccupied == false)
			m_boardpieces[(itr->m_id - 7)]->m_isPossibleMove = true;

		if ((itr->m_id - 14) <= m_boardpieces.size())
		{
			if (m_boardpieces[(itr->m_id - 7)]->m_isOccupied == true && m_boardpieces[(itr->m_id - 7)]->m_isGreen == true)
			{
				if (m_boardpieces[(itr->m_id - 14)]->m_isOccupied == false)
					m_boardpieces[(itr->m_id - 14)]->m_isPossibleMove = true;
			}
		}
	}

	if ((itr->m_id - 9) >= 0)
	{
		// Sets the Possible moves of the piece selected ------------------------------------------------------------------------
		if (m_boardpieces[(itr->m_id - 9)]->m_isBlack == false && m_boardpieces[(itr->m_id - 9)]->m_isOccupied == false)
			m_boardpieces[(itr->m_id - 9)]->m_isPossibleMove = true;

		if ((itr->m_id - 18) <= m_boardpieces.size())
		{
			if (m_boardpieces[(itr->m_id - 9)]->m_isOccupied == true && m_boardpieces[(itr->m_id - 9)]->m_isGreen == true)
			{
				if (m_boardpieces[(itr->m_id - 18)]->m_isOccupied == false)
					m_boardpieces[(itr->m_id - 18)]->m_isPossibleMove = true;
			}
		}
	}
}

void Checkerboard::ResetPossible(BoardPiece* itr)
{
	itr->m_isSelected = false;

	if (itr->m_isGreen == true)
	{
		if ((itr->m_id + 7) <= m_boardpieces.size())
			m_boardpieces[(itr->m_id + 7)]->m_isPossibleMove = false;

		if ((itr->m_id + 9) <= m_boardpieces.size())
			m_boardpieces[(itr->m_id + 9)]->m_isPossibleMove = false;
	}

	else if (itr->m_isPurple == true)
	{
		if ((itr->m_id - 7) <= m_boardpieces.size())
			m_boardpieces[(itr->m_id - 7)]->m_isPossibleMove = false;

		if ((itr->m_id - 9) <= m_boardpieces.size())
			m_boardpieces[(itr->m_id - 9)]->m_isPossibleMove = false;
	}



	m_selectedPieceID = NULL;
}

