#pragma once
class ActorBase
{
public:
	ActorBase();
	~ActorBase();

protected:
	//#TODO: ObjectUniBuff
	// stores:
	// MVP + various object related GPU stuff

	struct OpenGLInfo
	{
		unsigned int m_VAO;
		unsigned int m_VBO;
		unsigned int m_IBO;
		unsigned int m_index_count;
	};

	std::vector<OpenGLInfo> m_GLInfo;
	glm::mat4* m_ModelViewProjection;
};

