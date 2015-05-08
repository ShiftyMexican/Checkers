/*
	Author: Jarrod Dowsey	
	Description: Checkers Assessment
*/

#ifndef	CHECKERS_H
#define CHECKERS_H

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Gizmos.h>
#include <fstream>
#include <string>

#include <glm/glm.hpp>
#include <glm/ext.hpp>


class Application;
class FreeCamera;
class Checkerboard;
class Checkers
{
public:

	Checkers(GLFWwindow* window);

	~Checkers();

	void Update(float deltaTime);

	void Draw();


private:

	FreeCamera* m_camera;

	Checkerboard* m_checkerBoard;

	GLFWwindow* m_window;

	glm::vec3 m_tempMousePos;

	POINT mousepos;

	bool m_clicked;

};

#endif
