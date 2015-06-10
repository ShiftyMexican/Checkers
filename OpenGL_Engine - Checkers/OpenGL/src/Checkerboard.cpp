#include "Checkerboard.h"
#include "BoardPiece.h"
#include "ClientApplication.h"

Checkerboard::Checkerboard(GLFWwindow* window, ClientApplication* client)
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

	m_greenTurn = true;

	m_mustTake = false;

	m_pieceTaken = false;

	m_window = window;

	m_selectedPieceID = NULL;

	CreateCheckersBoard();

	m_client = client;

	m_greenPiecesLeft = 12;
	m_purplePiecesLeft = 12;

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
			if ((*itr)->m_isOccupied == true && (*itr)->m_isSelected == false && (*itr)->m_isGreen == true && m_selectedPieceID == NULL && m_greenTurn == true && m_client->m_uiClientID == 1 || 
				(*itr)->m_isOccupied == true && (*itr)->m_isSelected == false && (*itr)->m_isGreenKing == true && m_selectedPieceID == NULL && m_greenTurn == true && m_client->m_uiClientID == 1)
			{
				// Sets the possible moves
				if ((*itr)->m_isGreenKing == false)
					SetGreenPossibleMoves((*itr));

				else if ((*itr)->m_isGreenKing == true)
					SetGreenKingPossibleMoves((*itr));

				m_selectedPieceID = (*itr)->m_id;
			}

			// Check to see if the piece you have clicked has a purple checker on it
			else if ((*itr)->m_isOccupied == true && (*itr)->m_isSelected == false && (*itr)->m_isPurple == true && m_selectedPieceID == NULL && m_greenTurn == false && m_client->m_uiClientID == 2 ||
				(*itr)->m_isOccupied == true && (*itr)->m_isSelected == false && (*itr)->m_isPurpleKing == true && m_selectedPieceID == NULL && m_greenTurn == false && m_client->m_uiClientID == 2)
			{
				// Sets the possible moves
				if ((*itr)->m_isPurpleKing == false)
					SetPurplePossibleMoves((*itr));

				else if ((*itr)->m_isPurpleKing == true)
					SetPurpleKingPossibleMoves((*itr));
				
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
				SetMove((*itr));

				if (m_pieceTaken == true)
				{
					if ((*itr)->m_isPurple == true)
					{
						m_greenPiecesLeft -= 1;
						CheckForGreenKill((*itr));
					}
						

					else if ((*itr)->m_isGreen == true)
					{
						m_purplePiecesLeft -= 1;
						CheckForPurpleKill((*itr));
					}
						
					m_pieceTaken = false;
					
				}

				if (m_mustTake == false)
				{
					m_selectedPieceID = NULL;
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
	Gizmos::addAABBFilled(glm::vec3(80, 0, 80), glm::vec3(90, 2.7, 90), glm::vec4(0.4, 0.2, 0, 1), nullptr);

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


		// Drawing Green Pieces---------------------------------------------------------------------------------------------------------------------------------------
		if ((*itr)->m_isOccupied == true && (*itr)->m_isGreen == true && (*itr)->m_isGreenKing == false)
		{
			Gizmos::addCylinderFilled(glm::vec3((*itr)->m_position.x, 5.2f, (*itr)->m_position.z), 0.8f * (*itr)->m_width, 2.0f, 20, glm::vec4(0.0f, 0.9f, 0.2f, 1.0f), nullptr);
		}
		else if ((*itr)->m_isOccupied == true && (*itr)->m_isGreen == true && (*itr)->m_isGreenKing == true)
		{
			Gizmos::addCylinderFilled(glm::vec3((*itr)->m_position.x, 5.2f, (*itr)->m_position.z), 0.8f * (*itr)->m_width, 2.0f, 20, glm::vec4(0, 0.9, 0.2, 1), nullptr);
			Gizmos::addCylinderFilled(glm::vec3((*itr)->m_position.x, 9.3f, (*itr)->m_position.z), 0.8f * (*itr)->m_width, 2.0f, 20, glm::vec4(0, 0.9, 0.2, 1), nullptr);
		}
		// -----------------------------------------------------------------------------------------------------------------------------------------------------------

		// Drawing Purple Pieces -------------------------------------------------------------------------------------------------------------------------------------
		if ((*itr)->m_isOccupied == true && (*itr)->m_isPurple == true && (*itr)->m_isPurpleKing == false)
		{
			Gizmos::addCylinderFilled(glm::vec3((*itr)->m_position.x, 5.2f, (*itr)->m_position.z), 0.8f * (*itr)->m_width, 2.0f, 20, glm::vec4(0.6, 0, 1, 1), nullptr);
		}
		else if ((*itr)->m_isOccupied == true && (*itr)->m_isPurple == true && (*itr)->m_isPurpleKing == true)
		{
			Gizmos::addCylinderFilled(glm::vec3((*itr)->m_position.x, 5.2f, (*itr)->m_position.z), 0.8f * (*itr)->m_width, 2.0f, 20, glm::vec4(0.6, 0, 1, 1), nullptr);
			Gizmos::addCylinderFilled(glm::vec3((*itr)->m_position.x, 9.3f, (*itr)->m_position.z), 0.8f * (*itr)->m_width, 2.0f, 20, glm::vec4(0.6, 0, 1, 1), nullptr);
		}
		// -----------------------------------------------------------------------------------------------------------------------------------------------------------

		if ((*itr)->m_isPossibleMove == true)
		{
			Gizmos::addAABBFilled((*itr)->m_position, glm::vec3((*itr)->m_width, 2, (*itr)->m_height), glm::vec4(1, 0.35, 0, 1), nullptr);
		}
	}
}

void Checkerboard::CreateCheckersBoard()
{
	for (float x = 0; x < BOARD_WIDTH; x++)
	{
		for (float y = 0; y < BOARD_WIDTH; y++)
		{

			float xPos = (0.5f * 10 * 2) + (x * 10 * 2);
			float yPos = (0.5f * 10 * 2) + (y * 10 * 2);

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

	if ((itr->m_id + 7) < m_boardpieces.size())
	{

		CheckForPurpleKill(itr);
		
		// Sets the Possible moves of the piece selected ------------------------------------------------------------------------
		if (m_boardpieces[(itr->m_id + 7)]->m_isBlack == false && m_boardpieces[(itr->m_id + 7)]->m_isOccupied == false && m_mustTake == false)
			m_boardpieces[(itr->m_id + 7)]->m_isPossibleMove = true;

	}

	if ((itr->m_id + 9) < m_boardpieces.size())
	{

		CheckForPurpleKill(itr);

		// Sets the Possible moves of the piece selected ------------------------------------------------------------------------
		if (m_boardpieces[(itr->m_id + 9)]->m_isBlack == false && m_boardpieces[(itr->m_id + 9)]->m_isOccupied == false && m_mustTake == false)
			m_boardpieces[(itr->m_id + 9)]->m_isPossibleMove = true;

	}

}

void Checkerboard::SetPurplePossibleMoves(BoardPiece* itr)
{
	itr->m_isSelected = true;

	if (itr->m_id >= 7)
	{
		CheckForGreenKill(itr);

		// Sets the Possible moves of the piece selected ------------------------------------------------------------------------
		if (m_boardpieces[(itr->m_id - 7)]->m_isBlack == false && m_boardpieces[(itr->m_id - 7)]->m_isOccupied == false && m_mustTake == false)
			m_boardpieces[(itr->m_id - 7)]->m_isPossibleMove = true;
	}

	if (itr->m_id >= 9)
	{
		CheckForGreenKill(itr);

		// Sets the Possible moves of the piece selected ------------------------------------------------------------------------
		if (m_boardpieces[(itr->m_id - 9)]->m_isBlack == false && m_boardpieces[(itr->m_id - 9)]->m_isOccupied == false && m_mustTake == false)
			m_boardpieces[(itr->m_id - 9)]->m_isPossibleMove = true;
	}
}

void Checkerboard::ResetPossible(BoardPiece* itr)
{
	itr->m_isSelected = false;
	m_mustTake = false;

	for (auto iter = m_boardpieces.begin(); iter != m_boardpieces.end(); iter++)
	{
		if ((*iter)->m_isPossibleMove == true)
			(*iter)->m_isPossibleMove = false;
	}

	m_selectedPieceID = NULL;
}

void Checkerboard::SetMove(BoardPiece* itr)
{
	itr->m_isOccupied = true;
	itr->m_isGreen = m_boardpieces[m_selectedPieceID]->m_isGreen;
	itr->m_isPurple = m_boardpieces[m_selectedPieceID]->m_isPurple;
	itr->m_isGreenKing = m_boardpieces[m_selectedPieceID]->m_isGreenKing;
	itr->m_isPurpleKing = m_boardpieces[m_selectedPieceID]->m_isPurpleKing;
	itr->m_sendMove = true;
	m_boardpieces[m_selectedPieceID]->m_isSelected = false;
	m_boardpieces[m_selectedPieceID]->m_isOccupied = false;
	m_boardpieces[m_selectedPieceID]->m_sendMove = true;
	m_mustTake = false;

	if (itr->m_isGreen == true || itr->m_isGreenKing == true)
	{
		m_greenTurn = false;
		m_client->m_yourTurn = false;
	}
	else if (itr->m_isPurple == true || itr->m_isPurpleKing == true)
	{
		m_greenTurn = true;
		m_client->m_yourTurn2 = false;
	}
	

	if (itr->m_id == m_selectedPieceID + 14)
		m_killPlus7 = true;
	else if (itr->m_id == m_selectedPieceID + 18)
		m_killPlus9 = true;
	else if (itr->m_id == m_selectedPieceID - 14)
		m_killMinus7 = true;
	else if (itr->m_id == m_selectedPieceID - 18)
		m_killMinus9 = true;


	if (m_killMinus7 == true)
	{
		m_boardpieces[m_selectedPieceID - 7]->m_isOccupied = false;
		m_boardpieces[m_selectedPieceID - 7]->m_isGreen = false;
		m_boardpieces[m_selectedPieceID - 7]->m_isPurple = false;
		m_boardpieces[m_selectedPieceID - 7]->m_isGreenKing = false;
		m_boardpieces[m_selectedPieceID - 7]->m_isPurpleKing = false;
		m_boardpieces[m_selectedPieceID - 7]->m_sendMove = true;
		m_killMinus7 = false;
		m_pieceTaken = true;
	}

	else if (m_killPlus7 == true)
	{
		m_boardpieces[m_selectedPieceID + 7]->m_isOccupied = false;
		m_boardpieces[m_selectedPieceID + 7]->m_isPurple = false;
		m_boardpieces[m_selectedPieceID + 7]->m_isGreen = false;
		m_boardpieces[m_selectedPieceID + 7]->m_isGreenKing = false;
		m_boardpieces[m_selectedPieceID + 7]->m_isPurpleKing = false;
		m_boardpieces[m_selectedPieceID + 7]->m_sendMove = true;
		m_killPlus7 = false;
		m_pieceTaken = true;
	}

	else if (m_killMinus9 == true)
	{
		m_boardpieces[m_selectedPieceID - 9]->m_isOccupied = false;
		m_boardpieces[m_selectedPieceID - 9]->m_isGreen = false;
		m_boardpieces[m_selectedPieceID - 9]->m_isPurple = false;
		m_boardpieces[m_selectedPieceID - 9]->m_isGreenKing = false;
		m_boardpieces[m_selectedPieceID - 9]->m_isPurpleKing = false;
		m_boardpieces[m_selectedPieceID - 9]->m_sendMove = true;
		m_killMinus9 = false;
		m_pieceTaken = true;
	}

	else if (m_killPlus9 == true)
	{
		m_boardpieces[m_selectedPieceID + 9]->m_isOccupied = false;
		m_boardpieces[m_selectedPieceID + 9]->m_isPurple = false;
		m_boardpieces[m_selectedPieceID + 9]->m_isGreen = false;
		m_boardpieces[m_selectedPieceID + 9]->m_isGreenKing = false;
		m_boardpieces[m_selectedPieceID + 9]->m_isPurpleKing = false;
		m_boardpieces[m_selectedPieceID + 9]->m_sendMove = true;
		m_killPlus9 = false;
		m_pieceTaken = true;
	}

	for (auto iter = m_boardpieces.begin(); iter != m_boardpieces.end(); iter++)
	{
		if ((*iter)->m_isPossibleMove == true)
			(*iter)->m_isPossibleMove = false;

		if ((*iter)->m_sendMove == true)
		{
			m_client->SendMoveToServer((*iter));
			(*iter)->m_sendMove = false;
		}
	}	

	GreenKingCheck(itr);
	PurpleKingCheck(itr);

	m_selectedPieceID = itr->m_id;
}

void Checkerboard::CheckForPurpleKill(BoardPiece* itr)
{
	if (itr->m_isGreenKing == false)
	{
		if ((itr->m_id + 7) < m_boardpieces.size())
		{
			if (m_boardpieces[(itr->m_id + 7)]->m_isOccupied == true && m_boardpieces[(itr->m_id + 7)]->m_isPurple == true)
			{
				if ((itr->m_id + 14) < m_boardpieces.size())
				{
					if (m_boardpieces[(itr->m_id + 14)]->m_isOccupied == false && m_boardpieces[(itr->m_id + 14)]->m_isBlack == false)
					{
						m_boardpieces[(itr->m_id + 14)]->m_isPossibleMove = true;
						//m_killPlus7 = true;
						m_mustTake = true;
					}
				}
			}
		}

		if ((itr->m_id + 9) < m_boardpieces.size())
		{
			if (m_boardpieces[(itr->m_id + 9)]->m_isOccupied == true && m_boardpieces[(itr->m_id + 9)]->m_isPurple == true)
			{
				if (((int)itr->m_id + 18) < m_boardpieces.size())
				{
					if (m_boardpieces[(itr->m_id + 18)]->m_isOccupied == false && m_boardpieces[(itr->m_id + 18)]->m_isBlack == false)
					{
						m_boardpieces[(itr->m_id + 18)]->m_isPossibleMove = true;
						//m_killPlus9 = true;
						m_mustTake = true;
					}
				}
			}
		}
	}

	else if (itr->m_isGreenKing == true)
	{
		if ((itr->m_id + 7) < m_boardpieces.size())
		{
			if (m_boardpieces[(itr->m_id + 7)]->m_isOccupied == true && m_boardpieces[(itr->m_id + 7)]->m_isPurple == true)
			{
				if ((itr->m_id + 14) < m_boardpieces.size())
				{
					if (m_boardpieces[(itr->m_id + 14)]->m_isOccupied == false && m_boardpieces[(itr->m_id + 14)]->m_isBlack == false)
					{
						m_boardpieces[(itr->m_id + 14)]->m_isPossibleMove = true;
						//m_killPlus7 = true;
						m_mustTake = true;
					}
				}
			}
		}

		if ((itr->m_id + 9) < m_boardpieces.size())
		{
			if (m_boardpieces[(itr->m_id + 9)]->m_isOccupied == true && m_boardpieces[(itr->m_id + 9)]->m_isPurple == true)
			{
				if ((itr->m_id + 18) < m_boardpieces.size())
				{
					if (m_boardpieces[(itr->m_id + 18)]->m_isOccupied == false && m_boardpieces[(itr->m_id + 18)]->m_isBlack == false)
					{
						m_boardpieces[(itr->m_id + 18)]->m_isPossibleMove = true;
						//m_killPlus9 = true;
						m_mustTake = true;
					}
				}
			}
		}

		if (itr->m_id >= 7)
		{
			if (m_boardpieces[(itr->m_id - 7)]->m_isOccupied == true && m_boardpieces[(itr->m_id - 7)]->m_isPurple == true)
			{
				if (((int)itr->m_id - 14) >= 0)
				{
					if (m_boardpieces[(itr->m_id - 14)]->m_isOccupied == false && m_boardpieces[(itr->m_id - 14)]->m_isBlack == false)
					{
						m_boardpieces[(itr->m_id - 14)]->m_isPossibleMove = true;
						//m_killMinus7 = true;
						m_mustTake = true;
					}
				}
			}
		}

		if (itr->m_id >= 9)
		{
			if (m_boardpieces[(itr->m_id - 9)]->m_isOccupied == true && m_boardpieces[(itr->m_id - 9)]->m_isPurple == true)
			{
				if (((int)itr->m_id - 18) >= 0)
				{
					if (m_boardpieces[(itr->m_id - 18)]->m_isOccupied == false && m_boardpieces[(itr->m_id - 18)]->m_isBlack == false)
					{
						m_boardpieces[(itr->m_id - 18)]->m_isPossibleMove = true;
						//m_killMinus9 = true;
						m_mustTake = true;
					}
				}
			}
		}
	}
}

void Checkerboard::CheckForGreenKill(BoardPiece* itr)
{
	if (itr->m_isPurpleKing == false)
	{
		if (((int)itr->m_id - 7) >= 0)
		{
			if (m_boardpieces[(itr->m_id - 7)]->m_isOccupied == true && m_boardpieces[(itr->m_id - 7)]->m_isGreen == true)
			{
				if (((int)itr->m_id - 14) >= 0)
				{
					if (m_boardpieces[(itr->m_id - 14)]->m_isOccupied == false && m_boardpieces[(itr->m_id - 14)]->m_isBlack == false)
					{
						m_boardpieces[(itr->m_id - 14)]->m_isPossibleMove = true;
						//m_killMinus7 = true;
						m_mustTake = true;
					}
				}
			}
		}

		if (((int)itr->m_id - 9) >= 0)
		{
			if (m_boardpieces[(itr->m_id - 9)]->m_isOccupied == true && m_boardpieces[(itr->m_id - 9)]->m_isGreen == true)
			{
				if (((int)itr->m_id - 18) >= 0)
				{
					if (m_boardpieces[(itr->m_id - 18)]->m_isOccupied == false && m_boardpieces[(itr->m_id - 18)]->m_isBlack == false)
					{
						m_boardpieces[(itr->m_id - 18)]->m_isPossibleMove = true;
						//m_killMinus9 = true;
						m_mustTake = true;
					}
				}
			}
		}
	}

	else if (itr->m_isPurpleKing == true)
	{
		if ((itr->m_id + 7) < m_boardpieces.size())
		{
			if (m_boardpieces[(itr->m_id + 7)]->m_isOccupied == true && m_boardpieces[(itr->m_id + 7)]->m_isGreen == true)
			{
				if ((itr->m_id + 14) < m_boardpieces.size())
				{
					if (m_boardpieces[(itr->m_id + 14)]->m_isOccupied == false && m_boardpieces[(itr->m_id + 14)]->m_isBlack == false)
					{
						m_boardpieces[(itr->m_id + 14)]->m_isPossibleMove = true;
						//m_killPlus7 = true;
						m_mustTake = true;
					}
				}
			}
		}

		if ((itr->m_id + 9) < m_boardpieces.size())
		{
			if (m_boardpieces[(itr->m_id + 9)]->m_isOccupied == true && m_boardpieces[(itr->m_id + 9)]->m_isGreen == true)
			{
				if ((itr->m_id + 18) < m_boardpieces.size())
				{
					if (m_boardpieces[(itr->m_id + 18)]->m_isOccupied == false && m_boardpieces[(itr->m_id + 18)]->m_isBlack == false)
					{
						m_boardpieces[(itr->m_id + 18)]->m_isPossibleMove = true;
						//m_killPlus9 = true;
						m_mustTake = true;
					}
				}
			}
		}

		if (((int)itr->m_id - 7) >= 0)
		{
			if (m_boardpieces[(itr->m_id - 7)]->m_isOccupied == true && m_boardpieces[(itr->m_id - 7)]->m_isGreen == true)
			{
				if (((int)itr->m_id - 14) >= 0)
				{
					if (m_boardpieces[(itr->m_id - 14)]->m_isOccupied == false && m_boardpieces[(itr->m_id - 14)]->m_isBlack == false)
					{
						m_boardpieces[(itr->m_id - 14)]->m_isPossibleMove = true;
						//m_killMinus7 = true;
						m_mustTake = true;
					}
				}
			}
		}

		if (((int)itr->m_id - 9) >= 0)
		{
			if (m_boardpieces[(itr->m_id - 9)]->m_isOccupied == true && m_boardpieces[(itr->m_id - 9)]->m_isGreen == true)
			{
				if (((int)itr->m_id - 18) >= 0)
				{
					if (m_boardpieces[(itr->m_id - 18)]->m_isOccupied == false && m_boardpieces[(itr->m_id - 18)]->m_isBlack == false)
					{
						m_boardpieces[(itr->m_id - 18)]->m_isPossibleMove = true;
						//m_killMinus9 = true;
						m_mustTake = true;
					}
				}
			}
		}
	}
}

void Checkerboard::GreenKingCheck(BoardPiece* itr)
{
	if (itr->m_isGreen == true && itr->m_id >= 56)
	{
		itr->m_isGreenKing = true;
	}
}

void Checkerboard::PurpleKingCheck(BoardPiece* itr)
{
	if (itr->m_isPurple == true && itr->m_id < 8)
	{
		itr->m_isPurpleKing = true;
	}
}

void Checkerboard::SetGreenKingPossibleMoves(BoardPiece* itr)
{
	itr->m_isSelected = true;

	if ((itr->m_id + 7) < m_boardpieces.size())
	{

		CheckForPurpleKill(itr);

		// Sets the Possible moves of the piece selected ------------------------------------------------------------------------
		if (m_boardpieces[(itr->m_id + 7)]->m_isBlack == false && m_boardpieces[(itr->m_id + 7)]->m_isOccupied == false && m_mustTake == false)
			m_boardpieces[(itr->m_id + 7)]->m_isPossibleMove = true;

	}

	if ((itr->m_id + 9) < m_boardpieces.size())
	{

		CheckForPurpleKill(itr);

		// Sets the Possible moves of the piece selected ------------------------------------------------------------------------
		if (m_boardpieces[(itr->m_id + 9)]->m_isBlack == false && m_boardpieces[(itr->m_id + 9)]->m_isOccupied == false && m_mustTake == false)
			m_boardpieces[(itr->m_id + 9)]->m_isPossibleMove = true;

	}

	itr->m_isSelected = true;

	if ((itr->m_id - 7) >= 0)
	{
		CheckForPurpleKill(itr);

		// Sets the Possible moves of the piece selected ------------------------------------------------------------------------
		if (m_boardpieces[(itr->m_id - 7)]->m_isBlack == false && m_boardpieces[(itr->m_id - 7)]->m_isOccupied == false && m_mustTake == false)
			m_boardpieces[(itr->m_id - 7)]->m_isPossibleMove = true;
	}

	if ((itr->m_id - 9) >= 0)
	{
		CheckForPurpleKill(itr);

		// Sets the Possible moves of the piece selected ------------------------------------------------------------------------
		if (m_boardpieces[(itr->m_id - 9)]->m_isBlack == false && m_boardpieces[(itr->m_id - 9)]->m_isOccupied == false && m_mustTake == false)
			m_boardpieces[(itr->m_id - 9)]->m_isPossibleMove = true;
	}
}

void Checkerboard::SetPurpleKingPossibleMoves(BoardPiece* itr)
{
	itr->m_isSelected = true;

	if (((int)itr->m_id + 7) < m_boardpieces.size())
	{

		CheckForGreenKill(itr);

		// Sets the Possible moves of the piece selected ------------------------------------------------------------------------
		if (m_boardpieces[(itr->m_id + 7)]->m_isBlack == false && m_boardpieces[(itr->m_id + 7)]->m_isOccupied == false && m_mustTake == false)
			m_boardpieces[(itr->m_id + 7)]->m_isPossibleMove = true;

	}

	if (((int)itr->m_id + 9) < m_boardpieces.size())
	{

		CheckForGreenKill(itr);

		// Sets the Possible moves of the piece selected ------------------------------------------------------------------------
		if (m_boardpieces[(itr->m_id + 9)]->m_isBlack == false && m_boardpieces[(itr->m_id + 9)]->m_isOccupied == false && m_mustTake == false)
			m_boardpieces[(itr->m_id + 9)]->m_isPossibleMove = true;

	}

	itr->m_isSelected = true;

	if ((itr->m_id - 7) >= 0)
	{
		CheckForGreenKill(itr);

		// Sets the Possible moves of the piece selected ------------------------------------------------------------------------
		if (m_boardpieces[(itr->m_id - 7)]->m_isBlack == false && m_boardpieces[(itr->m_id - 7)]->m_isOccupied == false && m_mustTake == false)
			m_boardpieces[(itr->m_id - 7)]->m_isPossibleMove = true;
	}

	if ((itr->m_id - 9) >= 0)
	{
		CheckForGreenKill(itr);

		// Sets the Possible moves of the piece selected ------------------------------------------------------------------------
		if (m_boardpieces[(itr->m_id - 9)]->m_isBlack == false && m_boardpieces[(itr->m_id - 9)]->m_isOccupied == false && m_mustTake == false)
			m_boardpieces[(itr->m_id - 9)]->m_isPossibleMove = true;
	}
}
