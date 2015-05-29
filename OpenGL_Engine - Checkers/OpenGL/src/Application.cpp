#include "Application.h"
#include "FreeCamera.h"
#include "Checkers.h"
#include "NetworkManager.h"
#include "ClientApplication.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;

// Constructor
Application::Application()
{
	// Calling of the Start up Function
	StartUp();
	//---------------------------------
	m_isActiveClient = false;
	m_isServer = false;

	// Initializing Gizmos
	Gizmos::create();

	// Initialization for the Free Camera ---------------------------------------
	myCamera = new FreeCamera(window);
	myCamera->SetupPerspective(glm::pi<float>() * 0.25f ,1240.0f / 768.0f);
	myCamera->LookAt(vec3(40, 0, 40), vec3(0), vec3(0, 1, 0));
	myCamera->SetFlySpeed(1000.0f);
	//---------------------------------------------------------------------------
	
	m_checkers = new Checkers(window);
}

// Destructor
Application::~Application()
{
	Gizmos::destroy();

	glfwDestroyWindow(window);
	glfwTerminate();
	return;
}

// Run Program - Main Function to call the Application to Run
void Application::Run()
{	
	// Main Game loop
	while(glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glEnable(GL_DEPTH_TEST);

		// Call Update
		Update();
		Draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

// Start Up Function - Contains Data needed to start Various things without the project
void Application::StartUp()
{
	if (glfwInit() == false)
		return;

	// Initializing the Screen size to 1240x768------------------------------------
	window = glfwCreateWindow(1240, 768, "Computer Graphics", nullptr, nullptr);
	//-----------------------------------------------------------------------------

	// Setting up the network------------------------------------------------------
	m_network = nullptr;
	m_client = nullptr;
	//m_network->Run();
	//-----------------------------------------------------------------------------

	// Deleteing the window if null------------------------------------------------
	if (window == nullptr)
	{
		glfwTerminate();
		return;
	}
	//-----------------------------------------------------------------------------

	// Making the Window context current-------------------------------------------
	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return;
	}
	//----------------------------------------------------------------------------

	// Setting the Colour of the window--------------------------------------------
	glClearColor(0, 0, 0, 1);
	//glEnable(GL_FRONT_AND_BACK);
	//-----------------------------------------------------------------------------
}

// Update Function
void Application::Update()
{
	// Creating DeltaTime Calculations-----------------------------------------
	m_currentTime = (float)glfwGetTime();
	deltaTime = m_currentTime - m_previousTime; 
	m_previousTime = m_currentTime;
	//-------------------------------------------------------------------------

	if (m_isServer == false && m_isActiveClient == false)
	{
		char str[512];

		printf("(C)lient or (S)erver?\n");
		gets(str);

		if ((str[0] == 'c') || (str[0] == 'C'))
		{
			m_isServer = false;
			m_isActiveClient = true;
			m_client = new ClientApplication();
		}
		else if ((str[0] == 's') || (str[0] == 'S'))
		{
			m_isServer = true;
			m_network = new NetworkManager();
		}

		if (m_isServer == true)
		{
			m_network->Run();
		}
		else
		{
			printf("enter sever IP or hit enter for 127.0.0.1\n");
			gets(str);

			if (str[0] == 0)
			{
				strcpy(str, "127.0.0.1");
			}
			printf("starting the Client. \n");
			m_client->HandleNetworkConnection();
		}

	}

	m_checkers->Update(deltaTime);

	if (m_isServer == true)
	{
		m_network->Update();
	}

	if (m_isActiveClient == true)
		m_client->Update();
}

// Draw Function
void Application::Draw()
{
	m_checkers->Draw();
}

// Load Shader Function
unsigned int Application::LoadShader(const char* shaderName, unsigned int uiType)
{
	int success = GL_FALSE;

	unsigned int handle = glCreateShader(uiType);
	unsigned char* source = FileToBuffer(shaderName);

	glShaderSource(handle, 1, (const char**)&source, 0);
	glCompileShader(handle);

	glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
	if(success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetShaderInfoLog(handle, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);

		delete[] infoLog;
		return 0;
	}

	delete[] source;

	return handle;
}

// Reads the Shaders in from File
unsigned char* Application::FileToBuffer(const char* a_strName)
{
	// open file for text reading
	FILE* pFile = fopen(a_strName, "rb");
	if (pFile == nullptr)
	{
		printf_s("Error: Unable to open file '%s' for reading!\n", a_strName);
		return nullptr;
	}

	// get number of bytes in file
	fseek(pFile, 0, SEEK_END);
	unsigned int uiLength = (unsigned int)ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	// allocate buffer and read file contents
	unsigned char* acBuffer = new unsigned char[uiLength + 1];
	memset(acBuffer, 0, uiLength + 1);
	fread(acBuffer, sizeof(unsigned char), uiLength, pFile);

	acBuffer[uiLength] = '\0';

	fclose(pFile);
	return acBuffer;
}

// Hanldes the Shaders
unsigned int Application::HandleShader(const char* a_vertexShader, const char* a_fragmentShader, const char* a_geometryShader)
{
	GLuint vertexShader = 0;
	GLuint fragmentShader = 0;
	GLuint geometryShader = 0;
	GLuint updateShader = 0;
	// Loading my Shaders in --------------------------------------------------
	vertexShader = LoadShader(a_vertexShader, GL_VERTEX_SHADER);
	fragmentShader = LoadShader(a_fragmentShader, GL_FRAGMENT_SHADER);
	//-------------------------------------------------------------------------

	if (a_geometryShader != nullptr)
	{
		geometryShader = LoadShader(a_geometryShader, GL_GEOMETRY_SHADER);
	}

	unsigned int uiShaderID = CreateProgram(vertexShader, fragmentShader, geometryShader);

	return uiShaderID;
}

// Creates the programs and attatches shaders to them
unsigned int Application::CreateProgram(GLuint vs)
{
	int success = GL_FALSE;

	unsigned int programID = glCreateProgram();

	glAttachShader(programID, vs);

	glLinkProgram(programID);

	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetProgramInfoLog(programID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);

		delete[] infoLog;
		return 0;
	}

	glDeleteShader(vs);

	return programID;
}

// Creates the programs and attatches shaders to them 
unsigned int Application::CreateProgram(GLuint vs, GLuint fs, GLuint gs)
{
	int success = GL_FALSE;

	unsigned int programID = glCreateProgram();
	
	glAttachShader(programID, vs);
	glAttachShader(programID, fs);
	glAttachShader(programID, gs);

	glLinkProgram(programID);
	
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if(success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetProgramInfoLog(programID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);

		delete[] infoLog;
		return 0;
	}

	return programID;
}


