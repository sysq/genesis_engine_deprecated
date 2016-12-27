#include "stdneb.h"
#include "SkySphere.h"
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
	__ImplementClass(SkySphere,'SSPH', Core::RefCounted)

SkySphere::SkySphere()
: m_pVertices(NULL),
  m_nCircles(80),
  m_nSteps(70),
  m_SkydomeFadingPercent(0.05f),
  m_bSmoothSkydomeFading(true)
{

}

SkySphere::~SkySphere()
{

}

void SkySphere::SetUp()
{

//[zhongdaohuan][render_obj]临时删掉实现
}

void SkySphere::Destroy()
{
	m_VertexBuffer   = 0;
	m_primitiveGroup = 0;

	m_VertexComponents.Clear();

	if (m_pVertices != NULL)
	{
		n_delete_array(m_pVertices);
		m_pVertices = NULL;
	}
	
	m_CloudMaterial = NULL;
	m_MaterialVector.Clear();
}

void SkySphere::CreateSphere()
{
	//int numVertices = m_nSteps * m_nCircles + 1;
	//int numEle      = 6 * m_nSteps * (m_nCircles-1) + 3 * m_nSteps;

	//m_VertexComponents.Append(RenderBase::VertexComponent(RenderBase::VertexComponent::Position,0, RenderBase::VertexComponent::Float3));
	//m_VertexComponents.Append(RenderBase::VertexComponent(RenderBase::VertexComponent::TexCoord,0, RenderBase::VertexComponent::Float3));
	//m_VertexComponents.Append(RenderBase::VertexComponent(RenderBase::VertexComponent::TexCoord,1, RenderBase::VertexComponent::Float2));
	//m_VertexComponents.Append(RenderBase::VertexComponent(RenderBase::VertexComponent::TexCoord,2, RenderBase::VertexComponent::Float));


	//m_VertexBuffer = RenderBase::VertexBuffer::Create();

	//GPtr<RenderBase::IndexBuffer> ib = RenderBase::IndexBuffer::Create();

	//unsigned int *indexbuffer = n_new_array(unsigned int, numEle);


	//for (IndexT k = 0; k < m_nSteps; k++)
	//{
	//	indexbuffer[k*3] = 0;
	//	indexbuffer[k*3+1] = k+1;

	//	if (k != m_nSteps-1)
	//	{
	//		indexbuffer[k*3+2] = k+2;
	//	}
	//	else
	//	{
	//		indexbuffer[k*3+2] = 1;
	//	}
	//}

	//for(IndexT y=0; y<m_nCircles-1; y++) 
	//{
	//	for(IndexT x=0; x<m_nSteps; x++) 
	//	{
	//		unsigned int *twoface = indexbuffer + (y*m_nSteps+x)*6 + 3 * m_nSteps;

	//		int p0 = 1+y * m_nSteps + x ;
	//		int p1 = 1+y * m_nSteps + x + 1 ;
	//		int p2 = 1+(y+1)* m_nSteps + x ;
	//		int p3 = 1+(y+1)* m_nSteps + x + 1 ;

	//		if (x == m_nSteps-1)
	//		{
	//			p1 -= x+1;
	//			p3 -= x+1;
	//		}

	//		// First triangle
	//		twoface[2]=p0;
	//		twoface[1]=p1;
	//		twoface[0]=p2;

	//		// Second triangle
	//		twoface[5]=p1;
	//		twoface[4]=p3;
	//		twoface[3]=p2;
	//	}
	//}

	//ib->Setup(RenderBase::IndexType::Index32, numEle, indexbuffer, numEle * sizeof(unsigned int), 
	//	      RenderBase::IndexBuffer::UsageImmutable, RenderBase::IndexBuffer::AccessNone);

	//m_primitiveGroup = RenderBase::PrimitiveGroup::Create();
	//m_primitiveGroup->SetIndexBuffer(ib);

	//m_nIndiceCount = numEle;

	//n_delete_array(indexbuffer);

	//m_pVertices = n_new_array(POS_UV_VERTEX, 1 + m_nSteps * m_nCircles); 

	//m_nVerticeCount = 1 + m_nSteps * m_nCircles;

	//Math::bbox box = Math::bbox(Math::point(0.0,0.0,0.0), Math::vector(9999999.0,9999999.0,9999999.0));
	////SetBoundingBox(box);//[zhongdaohuan][render_obj]临时删掉实现
}

void SkySphere::UpdateSphere()
{
	/*float TODO_Radius = 1000;
	GPtr<Graphic::Camera> camera = Graphic::GraphicSystem::Instance()->GetCameraByType(Graphic::eCO_Main);
	if ( camera )
	{
		TODO_Radius =camera->GetCameraSetting().GetZFar() * 0.95f;
	}
	

	m_pVertices[0].pos.set(0.0f, TODO_Radius, 0.0f);
	m_pVertices[0].normal.set(0.0f, 1.0f, 0.0f);
	m_pVertices[0].uv.set(4.0f, 4.0f);
	m_pVertices[0].opacity = 1.0f;

	float AngleStep = float( (PI) / (m_nCircles - 1) );

	float r, uvr, c, s, sc;
	IndexT  x, y;

	for (y = 0; y<m_nCircles-1; y++)
	{
		r   = Math::n_cos(Math::scalar(PI/2 - AngleStep*(y+1)));
		uvr = static_cast<float>(y+1)/(m_nCircles-1);

		for(x = 0; x<m_nCircles; x++)
		{
			c  = Math::n_cos( Math::scalar(PI * 2.0f * x / m_nSteps) )  * r;
			s  = Math::n_sin( Math::scalar(PI * 2.0f * x / m_nSteps) )  * r;
			sc = Math::n_sin( Math::n_acos(r));

			if( PI/2 - AngleStep*(y+1) < 0.0f )
				sc = - sc ;

			m_pVertices[1+y*m_nSteps + x].pos.set(c * TODO_Radius, sc * TODO_Radius, s * TODO_Radius);
			
			m_pVertices[1+y*m_nSteps + x].normal.set(c, sc, s);

			m_pVertices[1+y*m_nSteps + x].uv.set((1 + c*uvr/r)*4, (1 + s*uvr/r)*4);

			m_pVertices[1+y*m_nSteps + x].opacity = 1.0f;
			
		
		}
	}

	r   = Math::n_cos(AngleStep);
	uvr = static_cast<float>(m_nCircles + 1) / (m_nCircles - 1);

	for(x = 0; x<m_nSteps; x++)
	{
		c = Math::n_cos(Math::scalar(PI * 2.0 * x / m_nSteps)) * r;
		s = Math::n_sin(Math::scalar(PI * 2.0 * x / m_nSteps)) * r;

		m_pVertices[1+(m_nCircles-1)*m_nSteps + x].pos.x() = m_pVertices[1+(m_nCircles-2)*m_nSteps + x].pos.x();
		m_pVertices[1+(m_nCircles-1)*m_nSteps + x].pos.y() = m_pVertices[1+(m_nCircles-2)*m_nSteps + x].pos.y() - TODO_Radius*m_SkydomeFadingPercent;
		m_pVertices[1+(m_nCircles-1)*m_nSteps + x].pos.z() = m_pVertices[1+(m_nCircles-2)*m_nSteps + x].pos.z();

		m_pVertices[1+(m_nCircles-1)*m_nSteps + x].normal.x() = m_pVertices[1+(m_nCircles-2)*m_nSteps + x].normal.x();
		m_pVertices[1+(m_nCircles-1)*m_nSteps + x].normal.y() = m_pVertices[1+(m_nCircles-2)*m_nSteps + x].normal.y();
		m_pVertices[1+(m_nCircles-1)*m_nSteps + x].normal.z() = m_pVertices[1+(m_nCircles-2)*m_nSteps + x].normal.z();

		m_pVertices[1+(m_nCircles-1)*m_nSteps + x].uv.x() = Math::scalar( (1 + c*uvr/r) * 4.0 );
		m_pVertices[1+(m_nCircles-1)*m_nSteps + x].uv.y() = Math::scalar( (1 + s*uvr/r) * 4.0 );

		m_pVertices[1+(m_nCircles-1)*m_nSteps + x].opacity = float( m_bSmoothSkydomeFading ? 0 : 1 );
	}



	m_VertexBuffer->Setup(m_VertexComponents, m_nVerticeCount, m_pVertices, m_nVerticeCount * sizeof(POS_UV_VERTEX), 
		RenderBase::VertexBuffer::UsageImmutable, RenderBase::VertexBuffer::AccessNone);

	m_primitiveGroup->SetVertexBuffer(m_VertexBuffer);
	m_primitiveGroup->SetBaseVertex(0);
	m_primitiveGroup->SetNumVertices(m_nVerticeCount);
	m_primitiveGroup->SetBaseIndex(0);
	m_primitiveGroup->SetNumIndices(m_nIndiceCount);
	m_primitiveGroup->SetPrimitiveTopology(RenderBase::PrimitiveTopology::TriangleList);

	if (m_primHandle.IsValid())
	{
		Graphic::GraphicSystem::Instance()->UpdatePrimitiveGroup(m_primHandle, m_primitiveGroup);
	}	
	else
		m_primHandle = Graphic::GraphicSystem::Instance()->CreatePrimitiveGroup(m_primitiveGroup);*/

}

}