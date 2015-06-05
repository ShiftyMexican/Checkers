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
#include "ClientApplication.h"
#include "RenderTarget.h"

class Application;
class FreeCamera;
class Checkerboard;
class Checkers
{
public:

	Checkers(Application* app, GLFWwindow* window, ClientApplication* client);

	~Checkers();

	void Update(float deltaTime);

	void Draw();

	unsigned int m_programID;

private:

	FreeCamera* m_camera;

	Checkerboard* m_checkerBoard;

	GLFWwindow* m_window;

	glm::vec3 m_tempMousePos;

	POINT mousepos;

	ClientApplication* m_client;

	Application* m_application;

	RenderTarget* m_renderTarget;

	bool m_clicked;

	bool m_lookAtWasSet;

};

#endif
