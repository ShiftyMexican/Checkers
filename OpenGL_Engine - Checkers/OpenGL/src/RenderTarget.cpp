#include "RenderTarget.h"
#include "stb_image.h"

RenderTarget::RenderTarget(unsigned int programID)
{
	m_programID = programID;

	int imageWidth = 0, imageHeight = 0, imageFormat = 0;

	unsigned char* data = stbi_load("YourTurn.jpg", &imageWidth, &imageHeight, &imageFormat, STBI_default);

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* data1 = stbi_load("YouWin.jpg", &imageWidth, &imageHeight, &imageFormat, STBI_default);

	glGenTextures(1, &m_texture2);
	glBindTexture(GL_TEXTURE_2D, m_texture2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* data2 = stbi_load("YouLose.jpg", &imageWidth, &imageHeight, &imageFormat, STBI_default);

	glGenTextures(1, &m_texture3);
	glBindTexture(GL_TEXTURE_2D, m_texture3);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(data);

	yourTurn = false;
	youWin = false;
	youWin2 = false;
	youLose = false;
	youLose2 = false;
}

RenderTarget::~RenderTarget()
{

}

void RenderTarget::Init()
{
	//glGenFramebuffers(1, &m_fbo);
	//glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	//
	//glGenTextures(1, &m_fboTexture);
	//glBindTexture(GL_TEXTURE_2D, m_fboTexture);
	//
	//glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 1240, 768);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_fboTexture, 0);
	//
	//glGenRenderbuffers(1, &m_fboDepth);
	//glBindRenderbuffer(GL_RENDERBUFFER, m_fboDepth);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1240, 768);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_fboDepth);
	//
	//GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	//glDrawBuffers(1, drawBuffers);
	//
	//
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_fboTexture, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		printf("Framebuffer Error!\n");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	float vertexData[] =
	{
		-1, -0.55, -1, 1, 0, 0,
		 1, -0.55, -1, 1, 1, 0,
		 1, -1, -1, 1, 1, 1,
		-1, -1, -1, 1, 0, 1,

	};

	unsigned int indexData[] =
	{
		0, 1, 2, 
		0, 2, 3,
	};

	//Create a new VAO
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// Generate GL Buffers
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, vertexData, GL_STATIC_DRAW);

	glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, ((char*)0) + 16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void RenderTarget::Draw()
{
	glClearColor(0, 0, 0, 0);
	glUseProgram(m_programID);

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, m_fboTexture);
	//glUniform1i(glGetUniformLocation(m_programID, "diffuse"), 0);

	if (youWin == true)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture2);
		unsigned int loc1 = glGetUniformLocation(m_programID, "myTexture");
		glUniform1i(loc1, 0);
	}
	else if (youWin2 == true)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture2);
		unsigned int loc1 = glGetUniformLocation(m_programID, "myTexture");
		glUniform1i(loc1, 0);
	}

	else if (youLose == true)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture3);
		unsigned int loc1 = glGetUniformLocation(m_programID, "myTexture");
		glUniform1i(loc1, 0);
	}

	else if (youLose2 == true)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture3);
		unsigned int loc1 = glGetUniformLocation(m_programID, "myTexture");
		glUniform1i(loc1, 0);
	}

	else if (yourTurn == true)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		unsigned int loc1 = glGetUniformLocation(m_programID, "myTexture");
		glUniform1i(loc1, 0);
	}


	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
	glClearColor(0, 0, 0, 1);
}

void RenderTarget::SetAsActiveRenderTarget()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glViewport(0, 0, 1240, 768);

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderTarget::ClearActiveRenderTarget()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 1240, 768);
}

