#include "GBuffer.h"

#include <glm\gtc\matrix_transform.hpp>

#include "gl_core_4_4.h"

#include "Input\Console\Console.h"
#include "Render\Objects\Components\Shape.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

GBuffer::GBuffer() :
	m_depthTexture(0),
	m_gBufferID(0)
{
	ZeroMemory(m_gTextures, GTEXTURES_NUM);
}

GBuffer::GBuffer(const unsigned int* a_programID) : GBuffer()
{
	m_pGBuffProgID = a_programID;
}

GBuffer::~GBuffer()
{
	if(m_gBufferID != NULL)
		glDeleteFramebuffers(1, &m_gBufferID);

	if(m_depthTexture != NULL)
		glDeleteTextures(1, &m_depthTexture);

	if(m_gTextures[0] != NULL)
		glDeleteTextures(ARRAY_SIZE_IN_ELEMENTS(m_gTextures), m_gTextures);

	if (m_postQuad != NULL)
		delete m_postQuad;
}

bool GBuffer::SetUp(const int a_wWidth, const int a_wHeight)
{
	m_postQuad = new Shape();
	m_postQuad->Create(Shape::Geometry::PLANE);

	glUseProgram(*m_pGBuffProgID);

	//#TODO: UTILISE MY TEXTURE CLASS INSTEAD!!
	glUniform1i(glGetUniformLocation(*m_pGBuffProgID, "gPosition")	, 0);
	glUniform1i(glGetUniformLocation(*m_pGBuffProgID, "gNormal")	, 1);
	glUniform1i(glGetUniformLocation(*m_pGBuffProgID, "gAlbedoSpec"), 2);
	glUniform1i(glGetUniformLocation(*m_pGBuffProgID, "gComposite")	, 3);

	// Create the FBO
	glGenFramebuffers(1, &m_gBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBufferID);

	// Create the texture for positional information
	glGenTextures(1, &m_gTextures[GTEXTURE_POSITION]);
	glBindTexture(GL_TEXTURE_2D, m_gTextures[GTEXTURE_POSITION]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, a_wWidth, a_wHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gTextures[GTEXTURE_POSITION], 0);
	
	// Create the texture for normal information
	glGenTextures(1, &m_gTextures[GTEXTURE_NORMAL]);
	glBindTexture(GL_TEXTURE_2D, m_gTextures[GTEXTURE_NORMAL]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, a_wWidth, a_wHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gTextures[GTEXTURE_NORMAL], 0);

	// Create the texture for albedo information
	glGenTextures(1, &m_gTextures[GTEXTURE_ALBEDO]);
	glBindTexture(GL_TEXTURE_2D, m_gTextures[GTEXTURE_ALBEDO]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, a_wWidth, a_wHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_gTextures[GTEXTURE_ALBEDO], 0);

	// Create the texture for albedo information
	glGenTextures(1, &m_gTextures[GTEXTURE_COMP]);
	glBindTexture(GL_TEXTURE_2D, m_gTextures[GTEXTURE_COMP]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, a_wWidth, a_wHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_gTextures[GTEXTURE_COMP], 0);

	// Define the buffers to render
	GLenum attachments[4] = {
		GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, 
		GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, attachments);

	// Create Depth Texture
	glGenTextures(1, &m_depthTexture);
	glBindTexture(GL_TEXTURE_2D, m_depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, a_wWidth, a_wHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthTexture, 0);

	// Check status of framebuffer
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		Console::Log(Console::FBACK::LOG_ERROR, "GBuffer failed.\n");
		return false;
	}
		
	Console::Log(Console::FBACK::LOG_SUCCESS, "GBuffer succeeded.\n");
	
	//Unbind buffers
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	return true;
}

void GBuffer::Render()
{
	glUseProgram(*m_pGBuffProgID);
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

	glUseProgram(*m_pGBuffProgID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_gTextures[GTEXTURE_POSITION]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_gTextures[GTEXTURE_NORMAL]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_gTextures[GTEXTURE_ALBEDO]);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_gTextures[GTEXTURE_COMP]);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}
