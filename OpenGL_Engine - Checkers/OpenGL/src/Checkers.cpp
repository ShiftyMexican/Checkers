#include "Checkers.h"
#include <Gizmos.h>
#include "FreeCamera.h"
#include "Checkerboard.h"
#include <iostream>
#include "Application.h"

Checkers::Checkers(Application* app, GLFWwindow* window, ClientApplication* client)
{
	m_camera = new FreeCamera(window);
	m_camera->SetupPerspective(glm::pi<float>() * 0.25f, 1240.0f / 768.0f);
	m_camera->SetFlySpeed(100.0f);

	m_window = window;

	m_clicked = false;

	m_checkerBoard = new Checkerboard(window, client);

	m_client = client;

	client->SetCheckerBoard(m_checkerBoard);

	m_lookAtWasSet = false;	

	m_application = app;
	
	m_programID = m_application->HandleShader("RenderTarget.vert", "RenderTarget.frag", 0);

	m_renderTarget = new RenderTarget(m_programID);
	m_renderTarget->Init();
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

	if (m_lookAtWasSet == false)
	{
		if (m_client->m_uiClientID == 1)
		{
			m_camera->LookAt(vec3(-90, 170, 80), vec3(80, 0, 80), vec3(0, 1, 0));
			m_lookAtWasSet = true;
		}
			
		else if (m_client->m_uiClientID == 2)
		{
			m_camera->LookAt(vec3(250, 170, 80), vec3(80, 0, 80), vec3(0, 1, 0));
			m_lookAtWasSet = true;
		}
			
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

	//m_renderTarget->Draw();

	Gizmos::draw(m_camera->GetProjectionView());
}
