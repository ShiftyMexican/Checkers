#include "Checkers.h"
#include <Gizmos.h>
#include "FreeCamera.h"
#include "Checkerboard.h"
#include <iostream>

Checkers::Checkers(GLFWwindow* window, ClientApplication* client)
{
	m_camera = new FreeCamera(window);
	m_camera->SetupPerspective(glm::pi<float>() * 0.25f, 1240.0f / 768.0f);
	m_camera->LookAt(vec3(-90, 170, 80), vec3(80, 0, 80), vec3(0, 1, 0));
	m_camera->SetFlySpeed(100.0f);

	// if player 1 LookAt = (-90, 170, 80);
	// if player 2 LookAt = (250, 170, 80);
	m_window = window;

	m_clicked = false;

	m_checkerBoard = new Checkerboard(window, client);

	client->SetCheckerBoard(m_checkerBoard);
	
}

Checkers::~Checkers()
{
	Gizmos::destroy();
}

void Checkers::Update(float deltaTime)
{
	double xPos, yPos;

	glfwGetCursorPos(m_window, &xPos, &yPos);
	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && m_clicked == false)
	{
		vec3 Tileloc = m_camera->pickAgainstPlane((float)xPos, (float)yPos, vec4(0, 1, 0, 0));
		m_tempMousePos = Tileloc;
	}

	m_camera->Update(deltaTime);
	m_checkerBoard->Update(m_tempMousePos);

	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		m_clicked = true;
	else
		m_clicked = false;
}

void Checkers::Draw()
{
	Gizmos::clear();

	m_checkerBoard->Draw();

	Gizmos::draw(m_camera->GetProjectionView());
}
