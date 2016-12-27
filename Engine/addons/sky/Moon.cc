#include "stdneb.h"
#include "Moon.h"
#include "SkyRender.h"
#include "GraphicSystem.h"

namespace Sky
{
	__ImplementClass(Moon,'MOON', Core::RefCounted)

Moon::Moon()
: m_bCreated(false),
  m_MoonSize(0.225f),
  m_pVertices(NULL),
  m_pIndex(NULL),
  m_nVerticeCount(4),
  m_nIndiceCount(6)
{

}

Moon::~Moon()
{

}

void Moon::Setup()
{
	CreateMoon();

	//m_renderer = MoonRenderer::Create();//[zhongdaohuan][render_obj]临时册掉实现
}

void Moon::CreateMoon()
{
	if (m_bCreated)
	{
		return;
	}

	//m_VertexComponents.Append(RenderBase::VertexComponent(RenderBase::VertexComponent::Position,0, RenderBase::VertexComponent::Float3));
	//m_VertexComponents.Append(RenderBase::VertexComponent(RenderBase::VertexComponent::TexCoord,0, RenderBase::VertexComponent::Float2));

	//m_VertexBuffer = RenderBase::VertexBuffer::Create();

	//GPtr<RenderBase::IndexBuffer> ib = RenderBase::IndexBuffer::Create();

	//m_pIndex = n_new_array(unsigned short, m_nIndiceCount);

	//m_pIndex[0] = 0;
	//m_pIndex[1] = 2;
	//m_pIndex[2] = 1;
	//m_pIndex[3] = 1;
	//m_pIndex[4] = 2;
	//m_pIndex[5] = 3;

	//ib->Setup(RenderBase::IndexType::Index16, m_nIndiceCount, m_pIndex, m_nIndiceCount * sizeof(unsigned short), 
	//		  RenderBase::IndexBuffer::UsageImmutable, RenderBase::IndexBuffer::AccessNone);


	//m_primitiveGroup = RenderBase::PrimitiveGroup::Create();
	//m_primitiveGroup->SetIndexBuffer(ib);


	//float radius = 1000;
	//GPtr<Graphic::Camera> pCam = Graphic::GraphicSystem::Instance()->GetCameraByType(Graphic::Camera::Main);	
	//if ( pCam.isvalid() )
	//{
	//	radius = pCam->GetCameraSetting().GetZFar() * 0.95f;
	//}

	//float size   = radius * m_MoonSize;

	//m_pVertices = n_new_array(POS_UV_VERTEX, m_nVerticeCount);

	//m_pVertices[0].pos = Math::float3(-size/2.0f, size/2.0f, 0.0f);
	//m_pVertices[0].uv  = Math::float2(0.0f, 0.0f);

	//m_pVertices[1].pos = Math::float3(size/2.0f, size/2.0f, 0.0f);
	//m_pVertices[1].uv  = Math::float2(1.0f, 0.0f);

	//m_pVertices[2].pos = Math::float3(-size/2.0f, -size/2.0f, 0.0f);
	//m_pVertices[2].uv  = Math::float2(0.0f, 1.0f);

	//m_pVertices[3].pos = Math::float3(size/2.0f, -size/2.0f, 0.0f);
	//m_pVertices[3].uv  = Math::float2(1.0f, 1.0f);

	//m_VertexBuffer->Setup(m_VertexComponents, m_nVerticeCount, m_pVertices, m_nVerticeCount * sizeof(POS_UV_VERTEX), 
	//					  RenderBase::VertexBuffer::UsageImmutable, RenderBase::VertexBuffer::AccessNone);

	//m_primitiveGroup->SetVertexBuffer(m_VertexBuffer);
	//m_primitiveGroup->SetBaseVertex(0);
	//m_primitiveGroup->SetNumVertices(m_nVerticeCount);
	//m_primitiveGroup->SetBaseIndex(0);
	//m_primitiveGroup->SetNumIndices(m_nIndiceCount);
	//m_primitiveGroup->SetPrimitiveTopology(RenderBase::PrimitiveTopology::TriangleList);

	//m_primHandle = Graphic::GraphicSystem::Instance()->CreatePrimitiveGroup(m_primitiveGroup);

	//Math::bbox box;
	//box.set(Math::point(-size/2.0f, -size/2.0f, -size/2.0f),
	//		Math::point(size/2.0f, size/2.0f, size/2.0f));
	////SetBoundingBox(box);//[zhongdaohuan][render_obj]临时册掉实现

	//m_bCreated = true;


}

void Moon::Destroy()
{
	if (!m_bCreated)
	{
		return;
	}

	m_VertexBuffer   = 0;
	m_primitiveGroup = 0;

	m_VertexComponents.Clear();

	if (m_pVertices != NULL)
	{
		n_delete_array(m_pVertices);
		m_pVertices = NULL;
	}

	if (m_pIndex != NULL)
	{
		n_delete_array(m_pIndex);
		m_pIndex = NULL;
	}

	m_bCreated = false;
}

}