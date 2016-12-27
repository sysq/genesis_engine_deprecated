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
	class SkyBox : Core::RefCounted//public Graphic::Renderable
{
	__DeclareSubClass(SkyBox, Core::RefCounted);

public:
	static const int s_BoxFaces;

	struct POS_UV_VERTEX
	{
		Math::float3	pos;
		Math::float3	normal;
		Math::float2	uv;
		float			uvIdx;
		float			opacity;
	};

	SkyBox();

	virtual ~SkyBox();

	virtual void   Setup();

	void   CreateBox();

	void   Destroy();

	void   SetMaterial(const GPtr<Graphic::Material>& mat);

	const RenderBase::PrimitiveHandle& GetPrimHandle() const;
protected:
	void _createvb();
	void _createib();

private:
	RenderBase::PrimitiveHandle        m_primHandle;
	GPtr<RenderBase::PrimitiveGroup>    m_primitiveGroup;

	Util::Array<RenderBase::VertexComponent> m_VertexComponents;
	GPtr<RenderBase::VertexBuffer>            m_VertexBuffer;
	GPtr<RenderBase::IndexBuffer>		     m_IndexBuffer;


	POS_UV_VERTEX*						m_pVertices;

	int									m_nVerticeCount;
	int									m_nIndiceCount;

	friend class SkyRenderer;

};

	//--------------------------------------------------------------------------------
	inline const RenderBase::PrimitiveHandle& SkyBox::GetPrimHandle() const
	{
		return m_primHandle;
	}

}