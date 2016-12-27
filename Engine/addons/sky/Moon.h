#pragma once

#include "foundation/core/refcounted.h"
#include "foundation/core/ptr.h"
#include "rendersystem/base/PrimitiveGroup.h"
#include "rendersystem/base/RenderStateObject.h"
#include "graphicsystem/Material/Material.h"
#include "util/array.h"
#include "math/float3.h"
#include "math/float2.h"

namespace Sky
{
	class Moon  : public Core::RefCounted //: public Graphic::Renderable
{
	__DeclareSubClass(Moon, Core::RefCounted);

public:

	struct POS_UV_VERTEX
	{
		Math::float3 pos;
		Math::float2 uv;
	};

	Moon();

	virtual ~Moon();

	virtual void Setup();

	void CreateMoon();

	void Destroy();

	void SetMaterial(const GPtr<Graphic::Material>& mat);

	const bool& IsCreated() const;

	const float& GetMoonSize() const;

private:
	RenderBase::PrimitiveHandle        m_primHandle;
	GPtr<RenderBase::PrimitiveGroup>    m_primitiveGroup;
	RenderBase::GPUFragmentHandle       m_GPUFragmentHandle;
	GPtr<RenderBase::RenderStateObject> m_renderStateObject;
	RenderBase::RenderTargetHandle     m_renderTargetHandle;

	Util::Array<RenderBase::VertexComponent> m_VertexComponents;
	GPtr<RenderBase::VertexBuffer>            m_VertexBuffer;

	POS_UV_VERTEX*                     m_pVertices;
	unsigned short*                    m_pIndex;
	int                                m_nVerticeCount;
	int                                m_nIndiceCount;

	bool                               m_bCreated;
	float                              m_MoonSize;

	friend class MoonRenderer;

};

inline void Moon::SetMaterial(const GPtr<Graphic::Material> &mat)
{
	n_error("zhongdaohuan");//[zhongdaohuan][render_obj]¡Ÿ ±…æµÙ µœ÷
	//Renderable::SetMaterial(mat);
}

inline const bool& Moon::IsCreated() const
{
	return m_bCreated;
}

inline const float& Moon::GetMoonSize() const
{
	return m_MoonSize;
}

}