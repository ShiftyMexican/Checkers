/*
	Author: Jarrod Dowsey
	Description: Class for the Boardpieces
*/

#ifndef	BOARDPIECE_H
#define BOARDPIECE_H

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Gizmos.h>
#include <fstream>
#include <string>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class BoardPiece
{
public:
	BoardPiece();
	~BoardPiece();

	glm::mat4* m_blockMatrix;

	float m_width;
	float m_height;

	bool m_isBlack;
	bool m_isWhite;

	bool m_isOccupied;

private:





};

#endif
