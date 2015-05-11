/*
	Author: Jarrod Dowsey
	Description: Class for the board itself 
*/

#ifndef	CHECKERBOARD_H
#define CHECKERBOARD_H

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Gizmos.h>
#include <fstream>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Checkers;
class BoardPiece;
class Checkerboard
{
public:
	Checkerboard(GLFWwindow* window);

	~Checkerboard();

	void Update(glm::vec3 position);

	void Draw();

	void CreateCheckersBoard();

	void ColourSwitch();

	void AddBoardPiece(int id, float xPos, float zPos, bool colour, bool occupied);

	void SetGreenPossibleMoves(BoardPiece* itr);

	void SetPurplePossibleMoves(BoardPiece* itr);

	void ResetPossible(BoardPiece* itr);

private:
	static const unsigned int BOARD_WIDTH = 8;

	glm::vec4 m_black;
	glm::vec4 m_white;
	glm::vec4 m_currentColour;

	glm::vec3 m_boardPieceCenter;

	std::vector<BoardPiece*> m_boardpieces;

	float m_xOffset;
	float m_yOffset;

	float m_pieceID;
	float m_iterations;

	int m_selectedPieceID;

	bool m_isBlack;
	bool m_occupied;
	bool m_clicked;

	GLFWwindow* m_window;
};

#endif