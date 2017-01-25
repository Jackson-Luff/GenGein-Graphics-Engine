#pragma once
#include <vector>

namespace FX {

	////
	// Here there be amazing fitlers. 
	////
	struct DefaultFilter
	{
	public:
		inline unsigned int GetID() { return ID; }
		inline void SetID(unsigned int a_ID) { ID = a_ID; }
	private:

		unsigned int ID;
	};

	struct FXAA : public DefaultFilter
	{
	public:
	private:
	};

	////
	// Here there be amazing lights. 
	////
	struct DefaultLight
	{
	public:
		inline unsigned int GetID() { return ID; }
		inline void SetID(unsigned int a_ID) { ID = a_ID; }
	private:

		unsigned int ID;
	};

	struct PointLight : public DefaultLight
	{
	public:
	private:
	};

	struct SpotLight : public DefaultLight
	{
	public:
	private:
	};

	struct DirectionalLight : public DefaultLight
	{
	public:
	private:
	};
}

class Shape;

class PostProcessor
{
public:
	PostProcessor();
	PostProcessor(const unsigned int*, const unsigned int*);
	~PostProcessor();

	//
	bool SetUp(const int, const int);
	//
	void Render();

	// Filter functioins
	//bool AddFilter(FX::DefaultFilter&);
	//bool RemoveFilter(FX::DefaultFilter&);

	//// Light functions
	//bool AddLight(FX::DefaultLight&);
	//bool RemoveLight(FX::DefaultLight&);

private:
	
	//
	Shape* m_postQuad;
	//
	const unsigned int* m_postProgID;
	//
	std::vector<FX::DefaultFilter> m_filters;
	std::vector<FX::DefaultLight> m_lights;
};

