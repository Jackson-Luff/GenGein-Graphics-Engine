#include "gl_core_4_4.h"

#include "Input\Console\Console.h"
#include "Render\Objects\Components\Shape.h"
#include "GBuffer.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

GBuffer::GBuffer(unsigned int* a_programID)
{
	m_programID = a_programID;
	m_depthTexture = 0;
	m_gBufferID = 0;
	ZeroMemory(m_gTextures, GTEXTURES_NUM);
}

GBuffer::~GBuffer()
{
	if(m_gBufferID != 0)
		glDeleteFramebuffers(1, &m_gBufferID);

	if(m_depthTexture != 0)
		glDeleteTextures(1, &m_depthTexture);

	if(m_gTextures[0] != 0)
		glDeleteTextures(ARRAY_SIZE_IN_ELEMENTS(m_gTextures), m_gTextures);

	if (m_postQuad != nullptr)
		delete m_postQuad;
}

bool GBuffer::SetUp(const int a_wWidth, const int a_wHeight)
{
	m_wWidth  = a_wWidth;
	m_wHeight = a_wHeight;

	m_postQuad = new Shape();
	m_postQuad->Create(Shape::PLANE);

	glUseProgram(*m_programID);

	//#TODO: UTILISE MY TEXTURE CLASS INSTEAD!!
	glUniform1i(glGetUniformLocation(*m_programID, "gPosition")		, 0);
	glUniform1i(glGetUniformLocation(*m_programID, "gNormal")		, 1);
	glUniform1i(glGetUniformLocation(*m_programID, "gAlbedoSpec")	, 2);

	// Create the FBO
	glGenFramebuffers(1, &m_gBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBufferID);

	// Create the texture for positional information
	glGenTextures(1, &m_gTextures[GTEXTURE_POSITION]);
	glBindTexture(GL_TEXTURE_2D, m_gTextures[GTEXTURE_POSITION]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_wWidth, m_wHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gTextures[GTEXTURE_POSITION], 0);
	
	// Create the texture for normal information
	glGenTextures(1, &m_gTextures[GTEXTURE_NORMAL]);
	glBindTexture(GL_TEXTURE_2D, m_gTextures[GTEXTURE_NORMAL]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_wWidth, m_wHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gTextures[GTEXTURE_NORMAL], 0);

	// Create the texture for albedo information
	glGenTextures(1, &m_gTextures[GTEXTURE_ALBEDO]);
	glBindTexture(GL_TEXTURE_2D, m_gTextures[GTEXTURE_ALBEDO]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_wWidth, m_wHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_gTextures[GTEXTURE_ALBEDO], 0);

	// Define the buffers to render
	GLenum attachments[3] = {
		GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	// Create Depth Texture
	glGenRenderbuffers(1, &m_depthTexture);
	glBindRenderbuffer(GL_TEXTURE_2D, m_depthTexture);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_wWidth, m_wHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthTexture);

	// Check status of framebuffer
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		Console::Log(Console::LOG_ERROR, "GBuffer failed.\n");
		return false;
	}
		
	Console::Log(Console::LOG_SUCCESS, "GBuffer succeeded.\n");
	
	//Unbind buffers
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	return true;
}

void GBuffer::Render()
{
	glUseProgram(*m_programID);
	m_postQuad->Render();
}

void GBuffer::BindForWriting()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_gBufferID);
}

void GBuffer::BindForReading()
{
	
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gBufferID);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glUseProgram(*m_programID);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, m_gTextures[GTEXTURE_POSITION]);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, m_gTextures[GTEXTURE_NORMAL]);
	//glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_2D, m_gTextures[GTEXTURE_ALBEDO]);	
	
	GLsizei HalfWidth = (GLsizei)(m_wWidth / 2.0f);
	GLsizei HalfHeight = (GLsizei)(m_wHeight / 2.0f);
	
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glBlitFramebuffer(0, 0, m_wWidth, m_wHeight,0, 0, HalfWidth, HalfHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	
	glReadBuffer(GL_COLOR_ATTACHMENT1);
	glBlitFramebuffer(0, 0, m_wWidth, m_wHeight,0, HalfHeight, HalfWidth, m_wHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	
	glReadBuffer(GL_COLOR_ATTACHMENT2);
	glBlitFramebuffer(0, 0, m_wWidth, m_wHeight,HalfWidth, HalfHeight, m_wWidth, m_wHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	
	glReadBuffer(GL_DEPTH_ATTACHMENT);
	glBlitFramebuffer(0, 0, m_wWidth, m_wHeight, HalfWidth, 0, m_wWidth, HalfHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}
