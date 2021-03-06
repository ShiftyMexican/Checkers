/*	
	Author: Jarrod Dowsey
	Description: Simple Applicatuion class
*/

#ifndef APPLICATION_H
#define APPLICATION_H

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Gizmos.h>
#include <fstream>
#include <string>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Object.h"
#include "NetworkManager.h"
#include "ClientApplication.h"


struct GridVertex {

	glm::vec4 position;
	glm::vec4 colour;
};

struct Vertex {

	glm::vec4 position;
	glm::vec2 texCoord;
	glm::vec3 normal;
	glm::vec4 tangent;
};

class Object;
class FreeCamera;
class Checkers;
class Application
{
public:

	// Constructor
	Application();

	// Destructor
	~Application();

	void Run();

	void Update();

	void Draw();

	void StartUp();

	float deltaTime;

	unsigned int HandleShader(const char* vertexShader, const char* fragmentShader, const char* a_geometryShader);

	unsigned int LoadShader(const char* shaderName, unsigned int uiType);

	unsigned char* FileToBuffer(const char* a_strName);

	unsigned int CreateProgram(GLuint vs, GLuint fs, GLuint gs);

	unsigned int CreateProgram(GLuint vs);

	unsigned int ShaderHotLoad();

	bool m_isServer;
	bool m_isActiveClient;

protected:
	// Window
	GLFWwindow* window;

	// Camera Object
	FreeCamera* myCamera;

	// Checkers
	Checkers* m_checkers;

	// Network Manager
	NetworkManager* m_network;

	// Client Application
	ClientApplication* m_client;

	float m_currentTime;
	float m_previousTime;


};

#endif