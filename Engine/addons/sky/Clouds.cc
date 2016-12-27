#include "stdneb.h"
#include "Clouds.h"

namespace Sky
{
	__ImplementClass( CloudLayer, 'CLLA', Core::RefCounted);

CloudLayer::CloudLayer()
: m_Options(Options())
{
	m_AmbientGradient = ColorGradient();
	m_AmbientGradient.AddCFrame(ColorGradient::ColorFrame(Math::float3(1.0f,1.0f,1.0f)*0.95f, 1.0f));
	m_AmbientGradient.AddCFrame(ColorGradient::ColorFrame(Math::float3(0.7f,0.7f,0.65f), 0.625f)); 
	m_AmbientGradient.AddCFrame(ColorGradient::ColorFrame(Math::float3(0.6f,0.55f,0.4f), 0.5625f));
	m_AmbientGradient.AddCFrame(ColorGradient::ColorFrame(Math::float3(0.6f,0.45f,0.3f)*0.4f, 0.5f));
	m_AmbientGradient.AddCFrame(ColorGradient::ColorFrame(Math::float3(0.5f,0.25f,0.25f)*0.1f, 0.45f));
	m_AmbientGradient.AddCFrame(ColorGradient::ColorFrame(Math::float3(0.2f,0.2f,0.3f)*0.1f, 0.35f));
	m_AmbientGradient.AddCFrame(ColorGradient::ColorFrame(Math::float3(0.2f,0.2f,0.5f)*0.15f, 0));

	m_SunGradient = ColorGradient();
	m_SunGradient.AddCFrame(ColorGradient::ColorFrame(Math::float3(1.0f,1.0f,1.0f)*0.95f, 1.0f));
	m_SunGradient.AddCFrame(ColorGradient::ColorFrame(Math::float3(1.0f,1.0f,1.0f)*0.8f, 0.75f));
	m_SunGradient.AddCFrame(ColorGradient::ColorFrame(Math::float3(0.8f,0.75f,0.55f)*1.3f, 0.5625f));
	m_SunGradient.AddCFrame(ColorGradient::ColorFrame(Math::float3(0.6f,0.5f,0.2f)*0.75f, 0.5f));
	m_SunGradient.AddCFrame(ColorGradient::ColorFrame(Math::float3(0.6f,0.5f,0.2f)*0.35f, 0.4725f));
	m_SunGradient.AddCFrame(ColorGradient::ColorFrame(Math::float3(0.5f,0.5f,0.5f)*0.15f, 0.45f));
	m_SunGradient.AddCFrame(ColorGradient::ColorFrame(Math::float3(0.2f,0.2f,0.25f)*0.5f, 0.3f));
	m_SunGradient.AddCFrame(ColorGradient::ColorFrame(Math::float3(0.5f,0.5f,0.5f)*0.35f, 0.0f));
}

CloudLayer::CloudLayer(const Options& o)
: m_Options(o)
{
	m_AmbientGradient = ColorGradient();
	m_AmbientGradient.AddCFrame(ColorGradient::ColorFrame(Math::float3(1.0f,1.0f,1.0f)*0.95f, 1.0f));
	m_AmbientGradient.AddCFrame(ColorGradient::ColorFrame(Math::float3(0.7f,0.7f,0.65f), 0.625f)); 
	m_AmbientGradient.AddCFrame(ColorGradient::ColorFrame(Math::float3(0.6f,0.55f,0.4f), 0.5625f));
	m_AmbientGradient.AddCFrame(ColorGradient::ColorFrame(Math::float3(0.6f,0.45f,0.3f)*0.4f, 0.5f));
	m_AmbientGradient.AddCFrame(ColorGradient::ColorFrame(Math::float3(0.5f,0.25f,0.25f)*0.1f, 0.45f));
	m_AmbientGradient.AddCFrame(ColorGradient::ColorFrame(Math::float3(0.2f,0.2f,0.3f)*0.1f, 0.35f));
	m_AmbientGradient.AddCFrame(ColorGradient::ColorFrame(Math::float3(0.2f,0.2f,0.5f)*0.15f, 0));

	m_SunGradient = ColorGradient();
	m_SunGradient.AddCFrame(ColorGradient::ColorFrame(Math::float3(1.0f,1.0f,1.0f)*0.95f, 1.0f));
	m_SunGradient.AddCFrame(ColorGradient::ColorFrame(Math::float3(1.0f,1.0f,1.0f)*0.8f, 0.75f));
	m_SunGradient.AddCFrame(ColorGradient::ColorFrame(Math::float3(0.8f,0.75,0.55f)*1.3f, 0.5625f));
	m_SunGradient.AddCFrame(ColorGradient::ColorFrame(Math::float3(0.6f,0.5f,0.2f)*0.75f, 0.5f));
	m_SunGradient.AddCFrame(ColorGradient::ColorFrame(Math::float3(0.6f,0.5f,0.2f)*0.35f, 0.4725f));
	m_SunGradient.AddCFrame(ColorGradient::ColorFrame(Math::float3(0.5f,0.5f,0.5f)*0.15f, 0.45f));
	m_SunGradient.AddCFrame(ColorGradient::ColorFrame(Math::float3(0.2f,0.2f,0.25f)*0.5f, 0.3f));
	m_SunGradient.AddCFrame(ColorGradient::ColorFrame(Math::float3(0.5f,0.5f,0.5f)*0.35f, 0.0f));
}

CloudLayer::~CloudLayer()
{

}

}