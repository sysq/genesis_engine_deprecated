#include "stdneb.h"
#include "SkyBox.h"
#include "RenderSystem.h"
#include "foundation/io/ioserver.h"
#include "foundation/io/textreader.h"
#include "addons/wjshader/WJShader.h"
#include "addons/wjshader/parser/WJShaderParser.h"
#include "RenderMessageProtocol.h"
#include "GraphicSystem.h"
#include "SkyRender.h"

namespace Sky
{
	const int SkyBox::s_BoxFaces = 6;

	__ImplementClass(SkyBox,'SSBX', Core::RefCounted)//[zhongdaohuan][render_obj]临时册掉实现

SkyBox::SkyBox()
: m_pVertices(NULL)  
{

}

SkyBox::~SkyBox()
{

}

void SkyBox::Setup()
{
	CreateBox();
	//m_renderer = SkyRenderer::Create();//[zhongdaohuan][render_obj]临时册掉实现
}

void SkyBox::Destroy()
{
	m_VertexBuffer   = 0;
	m_primitiveGroup = 0;

	m_VertexComponents.Clear();

	if (m_pVertices != NULL)
	{
		n_delete_array(m_pVertices);
		m_pVertices = NULL;
	}
	
}

void SkyBox::_createvb()
{
	m_nVerticeCount = s_BoxFaces * 4;

	m_VertexComponents.Append(RenderBase::VertexComponent(RenderBase::VertexComponent::Position,0, RenderBase::VertexComponent::Float3));
	m_VertexComponents.Append(RenderBase::VertexComponent(RenderBase::VertexComponent::Normal  ,0, RenderBase::VertexComponent::Float3));
	m_VertexComponents.Append(RenderBase::VertexComponent(RenderBase::VertexComponent::TexCoord,0, RenderBase::VertexComponent::Float2));
	m_VertexComponents.Append(RenderBase::VertexComponent(RenderBase::VertexComponent::TexCoord,1, RenderBase::VertexComponent::Float));
	m_VertexComponents.Append(RenderBase::VertexComponent(RenderBase::VertexComponent::TexCoord,2, RenderBase::VertexComponent::Float));

	m_VertexBuffer = RenderBase::VertexBuffer::Create();
	m_pVertices = n_new_array(POS_UV_VERTEX, m_nVerticeCount); 

	Math::bbox box = Math::bbox(Math::point(0.0,0.0,0.0), Math::vector(9999999.0,9999999.0,9999999.0));
	//SetBoundingBox(box);//[zhongdaohuan]临时册掉实现

	Math::scalar minX = box.pmin.x();
	Math::scalar minY = box.pmin.y();
	Math::scalar minZ = box.pmin.z();

	Math::scalar maxX = box.pmax.x();
	Math::scalar maxY = box.pmax.y();
	Math::scalar maxZ = box.pmax.z();

	POS_UV_VERTEX* tempVertices = m_pVertices;

	//      5-----------6
	//     /     /      /|
	//    /------------/ |
	//   /     /      /| |
	//  1------------2 |/|
	//  |     |      | / |
	//  |  4  |      |/| 7
	//  |-----|------| |/  
	//  |     |      | |
	//  0------------3/
	//

	//Math::float3 normal(0.0f, 0.0f, 1.0f);
	////front face
	//{
	//	tempVertices->normal = normal;
	//	tempVertices->uv.set(0.0f, 0.0f);
	//	tempVertices->uvIdx = 0.0f;
	//	tempVertices->opacity = 1.0f;
	//	tempVertices->pos.set(minX, minY, minZ);tempVertices++;//0

	//	tempVertices->normal = normal;
	//	tempVertices->uv.set(0.0f, 1.0f);
	//	tempVertices->uvIdx = 0.0f;
	//	tempVertices->opacity = 1.0f;
	//	tempVertices->pos.set(minX, maxY, minZ);tempVertices++;//1

	//	tempVertices->normal = normal;
	//	tempVertices->uv.set(1.0f, 1.0f);
	//	tempVertices->uvIdx = 0.0f;
	//	tempVertices->opacity = 1.0f;
	//	tempVertices->pos.set(maxX, maxY, minZ);tempVertices++;//2

	//	tempVertices->normal = normal;
	//	tempVertices->uv.set(1.0f, 0.0f);
	//	tempVertices->uvIdx = 0.0f;
	//	tempVertices->opacity = 1.0f;
	//	tempVertices->pos.set(maxX, minY, minZ);tempVertices++;//3
	//}
	////back face
	//{
	//	normal = Math::float3(0.0f, 0.0f, -1.0f);
	//	tempVertices->normal = normal;
	//	tempVertices->uv.set(0.0f, 0.0f);
	//	tempVertices->uvIdx = 1.0f;
	//	tempVertices->opacity = 1.0f;
	//	tempVertices->pos.set(minX, minY, maxZ);tempVertices++;//4

	//	tempVertices->normal = normal;
	//	tempVertices->uv.set(0.0f, 1.0f);
	//	tempVertices->uvIdx = 1.0f;
	//	tempVertices->opacity = 1.0f;
	//	tempVertices->pos.set(minX, maxY, maxZ);tempVertices++;//5

	//	tempVertices->normal = normal;
	//	tempVertices->uv.set(1.0f, 1.0f);
	//	tempVertices->uvIdx = 1.0f;
	//	tempVertices->opacity = 1.0f;
	//	tempVertices->pos.set(maxX, maxY, maxZ);tempVertices++;//6

	//	tempVertices->normal = normal;
	//	tempVertices->uv.set(1.0f, 0.0f);
	//	tempVertices->uvIdx = 1.0f;
	//	tempVertices->opacity = 1.0f;
	//	tempVertices->pos.set(maxX, minY, maxZ);tempVertices++;//7
	//}
	////left face
	//{
	//	normal = Math::float3(1.0f, 0.0f, 0.0f);
	//	tempVertices->normal = normal;
	//	tempVertices->uv.set(0.0f, 0.0f);
	//	tempVertices->uvIdx = 2.0f;
	//	tempVertices->opacity = 1.0f;
	//	tempVertices->pos.set(maxX, minY, minZ);tempVertices++;//3

	//	tempVertices->normal = normal;
	//	tempVertices->uv.set(0.0f, 1.0f);
	//	tempVertices->uvIdx = 2.0f;
	//	tempVertices->opacity = 1.0f;
	//	tempVertices->pos.set(maxX, maxY, minZ);tempVertices++;//2

	//	tempVertices->normal = normal;
	//	tempVertices->uv.set(1.0f, 1.0f);
	//	tempVertices->uvIdx = 2.0f;
	//	tempVertices->opacity = 1.0f;
	//	tempVertices->pos.set(maxX, maxY, maxZ);tempVertices++;//6

	//	tempVertices->normal = normal;
	//	tempVertices->uv.set(1.0f, 0.0f);
	//	tempVertices->uvIdx = 2.0f;
	//	tempVertices->opacity = 1.0f;
	//	tempVertices->pos.set(maxX, minY, maxZ);tempVertices++;//7
	//}
	//// right face
	//{
	//	normal = Math::float3(-1.0f, 0.0f, 0.0f);
	//	tempVertices->normal = normal;
	//	tempVertices->uv.set(0.0f, 0.0f);
	//	tempVertices->uvIdx = 3.0f;
	//	tempVertices->opacity = 1.0f;
	//	tempVertices->pos.set(minX, minY, minZ);tempVertices++;//0

	//	tempVertices->normal = normal;
	//	tempVertices->uv.set(0.0f, 1.0f);
	//	tempVertices->uvIdx = 3.0f;
	//	tempVertices->opacity = 1.0f;
	//	tempVertices->pos.set(minX, maxY, minZ);tempVertices++;//1

	//	tempVertices->normal = normal;
	//	tempVertices->uv.set(1.0f, 1.0f);
	//	tempVertices->uvIdx = 3.0f;
	//	tempVertices->opacity = 1.0f;
	//	tempVertices->pos.set(minX, maxY, maxZ);tempVertices++;//5

	//	tempVertices->normal = normal;
	//	tempVertices->uv.set(1.0f, 0.0f);
	//	tempVertices->uvIdx = 3.0f;
	//	tempVertices->opacity = 1.0f;
	//	tempVertices->pos.set(minX, minY, maxZ);tempVertices++;//4
	//}
	//// up face
	//{
	//	normal = Math::float3(0.0f, -1.0f, 0.0f);
	//	tempVertices->normal = normal;
	//	tempVertices->uv.set(0.0f, 0.0f);
	//	tempVertices->uvIdx = 4.0f;
	//	tempVertices->opacity = 1.0f;
	//	tempVertices->pos.set(minX, maxY, minZ);tempVertices++;//1

	//	tempVertices->normal = normal;
	//	tempVertices->uv.set(0.0f, 1.0f);
	//	tempVertices->uvIdx = 4.0f;
	//	tempVertices->opacity = 1.0f;
	//	tempVertices->pos.set(minX, maxY, maxZ);tempVertices++;//5

	//	tempVertices->normal = normal;
	//	tempVertices->uv.set(1.0f, 1.0f);
	//	tempVertices->uvIdx = 4.0f;
	//	tempVertices->opacity = 1.0f;
	//	tempVertices->pos.set(maxX, maxY, maxZ);tempVertices++;//6

	//	tempVertices->normal = normal;
	//	tempVertices->uv.set(1.0f, 0.0f);
	//	tempVertices->uvIdx = 4.0f;
	//	tempVertices->opacity = 1.0f;
	//	tempVertices->pos.set(maxX, maxY, minZ);tempVertices++;//2
	//}
	//// down face
	//{
	//	normal = Math::float3(0.0f, 1.0f, 0.0f);
	//	tempVertices->normal = normal;
	//	tempVertices->uv.set(0.0f, 0.0f);
	//	tempVertices->uvIdx = 5.0f;
	//	tempVertices->opacity = 1.0f;
	//	tempVertices->pos.set(minX, minY, minZ);tempVertices++;//0

	//	tempVertices->normal = normal;
	//	tempVertices->uv.set(0.0f, 1.0f);
	//	tempVertices->uvIdx = 5.0f;
	//	tempVertices->opacity = 1.0f;
	//	tempVertices->pos.set(minX, minY, maxZ);tempVertices++;//4

	//	tempVertices->normal = normal;
	//	tempVertices->uv.set(1.0f, 1.0f);
	//	tempVertices->uvIdx = 5.0f;
	//	tempVertices->opacity = 1.0f;
	//	tempVertices->pos.set(maxX, minY, maxZ);tempVertices++;//7

	//	tempVertices->normal = normal;
	//	tempVertices->uv.set(1.0f, 0.0f);
	//	tempVertices->uvIdx = 5.0f;
	//	tempVertices->opacity = 1.0f;
	//	tempVertices->pos.set(maxX, minY, minZ);//3

	//}
	//m_VertexBuffer->Setup(m_VertexComponents, m_nVerticeCount, m_pVertices, m_nVerticeCount * sizeof(POS_UV_VERTEX), 
	//	RenderBase::VertexBuffer::UsageImmutable, RenderBase::VertexBuffer::AccessNone);

	//m_primitiveGroup->SetVertexBuffer(m_VertexBuffer);

}
void SkyBox::_createib()
{
	//// build and setup a ib
	//m_nIndiceCount  = s_BoxFaces * 6;
	//m_IndexBuffer = RenderBase::IndexBuffer::Create();
	//unsigned int *ib = n_new_array(unsigned int, m_nIndiceCount);
	//ib[0]  = 0; ib[1]  = 3; ib[2]  = 2; ib[3]  = 2; ib[4]  = 1; ib[5]  = 0;
	//ib[6]  = 7; ib[7]  = 6; ib[8]  = 5; ib[9]  = 5; ib[10] = 4; ib[11] = 7;
	//ib[12] = 3; ib[13] = 7; ib[14] = 6; ib[15] = 6; ib[16] = 2; ib[17] = 3;
	//ib[18] = 4; ib[19] = 0; ib[20] = 1; ib[21] = 1; ib[22] = 5; ib[23] = 4;
	//ib[24] = 1; ib[25] = 2; ib[26] = 6; ib[27] = 6; ib[28] = 5; ib[29] = 1;
	//ib[30] = 7; ib[31] = 4; ib[32] = 0; ib[33] = 0; ib[34] = 3; ib[35] = 7;

	//m_IndexBuffer->Setup(RenderBase::IndexType::Index32, m_nIndiceCount, ib, m_nIndiceCount * sizeof(unsigned int), 
	//	RenderBase::IndexBuffer::UsageImmutable, RenderBase::IndexBuffer::AccessNone);

	//m_primitiveGroup->SetIndexBuffer(m_IndexBuffer);	
	//n_delete_array(ib);
}

void SkyBox::CreateBox()
{
	//m_primitiveGroup = RenderBase::PrimitiveGroup::Create();
	//if ( !m_primitiveGroup.isvalid())
	//	return;

	//_createvb();
	//_createib();
	//m_primitiveGroup->SetBaseVertex(0);
	//m_primitiveGroup->SetNumVertices(m_nVerticeCount);
	//m_primitiveGroup->SetBaseIndex(0);
	//m_primitiveGroup->SetNumIndices(m_nIndiceCount);
	//m_primitiveGroup->SetPrimitiveTopology(RenderBase::PrimitiveTopology::TriangleList);

	//if (m_primHandle.IsValid())
	//{
	//	Graphic::GraphicSystem::Instance()->UpdatePrimitiveGroup(m_primHandle, m_primitiveGroup);
	//}	
	//else
	//	m_primHandle = Graphic::GraphicSystem::Instance()->CreatePrimitiveGroup(m_primitiveGroup);

}

}