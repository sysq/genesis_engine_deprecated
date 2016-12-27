#pragma once

#include "foundation/core/refcounted.h"
#include "foundation/core/ptr.h"
#include "rendersystem/base/PrimitiveGroup.h"
#include "rendersystem/base/RenderStateObject.h"
#include "graphicsystem/Material/Material.h"
#include "util/array.h"
#include "math/float3.h"

namespace Sky
{
	class SkySphere  : public Core::RefCounted //: public Graphic::Renderable
{
	__DeclareSubClass(SkySphere, Core::RefCounted);

public:
	struct POS_UV_VERTEX
	{
		Math::float3  pos;
		Math::float3  normal;
		Math::float2  uv;
		float         opacity;
	};

	SkySphere();

	virtual ~SkySphere();

	void   SetUp();

	void   CreateSphere();

	void   UpdateSphere();

	void   Destroy();

	void   SetMaterial(const GPtr<Graphic::Material>& mat);

	void   SetCloudMaterial(const GPtr<Graphic::Material>& mat);

	void   SetSphereParameter(int steps, int circles);

	void   SetSkydomeFadingParameters(bool SmoothSkydomeFading, float SkydomeFadingPercent = 0.05);

	const  int& GetSteps() const;

	const  int& GetCircles() const;

	const  bool& GetSmoothSkydomeFading() const;

	const  float& GetSkydomeFadingPercent() const;

	const RenderBase::PrimitiveHandle& GetPrimHandle() const;

private:
	RenderBase::PrimitiveHandle        m_primHandle;
	GPtr<RenderBase::PrimitiveGroup>    m_primitiveGroup;
	RenderBase::GPUFragmentHandle       m_GPUFragmentHandle;
	GPtr<RenderBase::RenderStateObject> m_renderStateObject;
	RenderBase::RenderTargetHandle     m_renderTargetHandle;

	Util::Array<RenderBase::VertexComponent> m_VertexComponents;
	GPtr<RenderBase::VertexBuffer>            m_VertexBuffer;


	POS_UV_VERTEX*                     m_pVertices;

	int                                m_nCircles;
	int                                m_nSteps;
	int                                m_nVerticeCount;
	int                                m_nIndiceCount;

	float                              m_SkydomeFadingPercent;
	bool                               m_bSmoothSkydomeFading;

	GPtr<Graphic::Material>             m_CloudMaterial;

	Util::Array< GPtr<Graphic::Material> >     m_MaterialVector;

	friend class SkyRenderer;

};

inline void SkySphere::SetSphereParameter(int steps, int circles)
{
	m_nSteps   = steps;
	m_nCircles = circles;
}

inline void SkySphere::SetSkydomeFadingParameters(bool SmoothSkydomeFading, float SkydomeFadingPercent)
{
	m_SkydomeFadingPercent = SkydomeFadingPercent;
	m_bSmoothSkydomeFading = SmoothSkydomeFading;
}

inline const int& SkySphere::GetSteps() const
{
	return m_nSteps;
}

inline const int& SkySphere::GetCircles() const
{
	return m_nCircles;
}

inline const bool& SkySphere::GetSmoothSkydomeFading() const
{
	return m_bSmoothSkydomeFading;
}

inline const float& SkySphere::GetSkydomeFadingPercent() const
{
	return m_SkydomeFadingPercent;
}

inline const RenderBase::PrimitiveHandle& SkySphere::GetPrimHandle() const
{
	return m_primHandle;
}

inline void SkySphere::SetMaterial(const GPtr<Graphic::Material>& mat)
{
	n_error("zhongdaohuan");//[zhongdaohuan][render_obj]¡Ÿ ±…æµÙ µœ÷
}

inline void SkySphere::SetCloudMaterial(const GPtr<Graphic::Material>& mat)
{
	m_CloudMaterial = mat;
	m_MaterialVector.Append(m_CloudMaterial);
}

}