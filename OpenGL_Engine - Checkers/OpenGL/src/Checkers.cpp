#include "Checkers.h"
#include <Gizmos.h>
#include "FreeCamera.h"
#include "Checkerboard.h"

Checkers::Checkers(GLFWwindow* window)
{
	m_camera = new FreeCamera(window);
	m_camera->SetupPerspective(glm::pi<float>() * 0.25f, 1240.0f / 768.0f);
	m_camera->LookAt(vec3(0, 300, 0), vec3(80, 0, 80), vec3(0, 1, 0));
	m_camera->SetFlySpeed(100.0f);

	m_checkerBoard = new Checkerboard();
}

Checkers::~Checkers()
{
	Gizmos::destroy();
}

void Checkers::Update(float deltaTime)
{
	m_camera->Update(deltaTime);
}

void Checkers::Draw()
{
	Gizmos::clear();

	m_checkerBoard->Draw();

	Gizmos::draw(m_camera->GetProjectionView());
}