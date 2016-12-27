#include "stdneb.h"
#include "MeshManager.h"
#include "GraphicSystem.h"

namespace Sky
{
	__ImplementClass( MeshManager, 'MMGR', Core::RefCounted);

MeshManager::MeshManager()
: m_bIsCreated(false)
{
//[zhongdaohuan][render_obj]临时册掉实现
}

MeshManager::~MeshManager()
{
	
}

void MeshManager::CreateGeometry()
{
//[zhongdaohuan][render_obj]临时册掉实现
}

void MeshManager::UpdateGeometry()
{
	if (m_SkySphere.isvalid())
	{
		m_SkySphere->UpdateSphere();
	}
	
}

void MeshManager::DestroyGeometry()
{
	if (m_SkySphere.isvalid())
	{
		m_SkySphere->Destroy();
		m_pMoon->Destroy();

		m_SkySphere = 0;
		m_pMoon     = 0;

		m_bIsCreated = false;
	}
}

void MeshManager::SetGeometryParameters(const int &Steps, const int &Circles)
{
	if (m_SkySphere.isvalid())
	{
		m_SkySphere->SetSphereParameter(Steps, Circles);

	}
	
	if (m_bIsCreated)
	{
		DestroyGeometry();

		CreateGeometry();	

	}
}

}