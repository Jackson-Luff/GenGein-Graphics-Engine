#include "PostProcess.h"

#include <algorithm>
#include "Render\Objects\Components\Shape.h"


PostProcessor::PostProcessor()
{}

PostProcessor::PostProcessor(const unsigned int* a_postProgID,
	const unsigned int* a_gBuffProgID)
{
	m_postProgID = a_postProgID;


}

PostProcessor::~PostProcessor()
{
	if (m_postQuad != nullptr)
		delete m_postQuad;

}

bool PostProcessor::SetUp(const int a_wWidth, const int a_wHeight)
{
	bool result = false;

	m_postQuad = new Shape();
	m_postQuad->Create(Shape::Geometry::PLANE);
	
	return result;
}

void PostProcessor::Render()
{	
	m_postQuad->Render();
}

//bool PostProcessor::AddFilter(FX::DefaultFilter& a_filter)
//{
//	std::vector<FX::DefaultFilter>::iterator position = std::find(m_filters.begin(), m_filters.end(), a_filter);
//	if (!(position != m_filters.end()))
//		m_filters.push_back(a_filter);
//}
//
//bool PostProcessor::RemoveFilter(FX::DefaultFilter& a_filter)
//{
//	std::vector<FX::DefaultFilter>::iterator position = std::find(m_filters.begin(), m_filters.end(), a_filter);
//	if (position != m_filters.end())
//		m_filters.erase(position);
//}
//
//bool PostProcessor::AddLight(FX::DefaultLight& a_light)
//{
//	std::vector<FX::DefaultLight>::iterator position = std::find(m_lights.begin(), m_lights.end(), a_light);
//	if (!(position != m_lights.end()))
//		m_lights.push_back(a_light);
//}
//
//bool PostProcessor::RemoveLight(FX::DefaultLight& a_light)
//{
//	std::vector<FX::DefaultLight>::iterator position = std::find(m_lights.begin(), m_lights.end(), a_light);
//	if (position != m_lights.end())
//		m_lights.erase(position);
//}