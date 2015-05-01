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

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Checkers;
class BoardPiece;
class Checkerboard
{
public:
	Checkerboard();

	~Checkerboard();

	void Update();

	void Draw();

	void CreateCheckersBoard();

	void ColourSwitch();

private:
	static const unsigned int BOARD_WIDTH = 8;

	glm::vec4 m_black;
	glm::vec4 m_white;

	glm::vec4 m_currentColour;

	float m_xOffset;
	float m_yOffset;

	float m_iterations;

	BoardPiece* m_boardpiece;


};

#endif