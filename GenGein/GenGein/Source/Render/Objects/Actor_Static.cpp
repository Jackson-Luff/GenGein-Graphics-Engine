#include "Actor_Static.h"

Actor_Static::Actor_Static()
{
	
}

Actor_Static::Actor_Static(int a_programID)
{
}

Actor_Static::~Actor_Static()
{
	m_GLInfo.clear();
	delete m_ModelViewProjection;
}

//MERGE OpenGLBufferCreation here for multiuse