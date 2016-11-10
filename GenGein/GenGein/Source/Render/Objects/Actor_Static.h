#pragma once
#include <vector>
#include <glm\fwd.hpp>

#include "ActorBase.h"

class Mesh;

class Actor_Static : ActorBase
{
public:
	Actor_Static();
	Actor_Static(int a_programID);
	~Actor_Static();

	virtual void Load() = 0;
	virtual void Render() = 0;
protected:
	Mesh* m_mesh;

};