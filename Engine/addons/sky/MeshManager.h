#pragma once

#include "sky/SkySphere.h"
#include "sky/Moon.h"
#include "math/matrix44.h"

namespace Sky
{
	class MeshManager : public Core::RefCounted
{
	__DeclareClass(MeshManager);

public:

	MeshManager();

	~MeshManager();

	void CreateGeometry();

	void UpdateGeometry();

	void DestroyGeometry();

	void SetGeometryParameters(const int &Steps, const int &Circles);

	const bool& IsCreated() const;

	const GPtr<SkySphere>& GetSphereMeshRes() const;

	const GPtr<Moon>& GetMoonMeshRes() const;

	void SetSphereTrans(const Math::matrix44& trans);

	const Math::matrix44& GetSphereTrans() const; 

	void SetMoonTrans(const Math::matrix44& trans);

	const Math::matrix44& GetMoonTrans() const;

private:
	GPtr<SkySphere>       m_SkySphere;
	GPtr<Moon>            m_pMoon;

	bool                 m_bIsCreated;
};

inline const bool& MeshManager::IsCreated() const
{
	return m_bIsCreated;
}

inline const GPtr<SkySphere>& MeshManager::GetSphereMeshRes() const
{
	return m_SkySphere;
}

inline void MeshManager::SetSphereTrans(const Math::matrix44 &trans)
{
	n_error("zhongdaohuan");//[zhongdaohuan][render_obj]临时删掉实现
	//m_SkySphere->SetTransform(trans);
}

inline const Math::matrix44& MeshManager::GetSphereTrans() const
{
	n_error("zhongdaohuan");//[zhongdaohuan][render_obj]临时删掉实现
	//return m_SkySphere->GetTransform();	
	static Math::matrix44 identity = Math::matrix44::identity();
	return identity;
}

inline const GPtr<Moon>& MeshManager::GetMoonMeshRes() const
{
	return m_pMoon;
}

inline void MeshManager::SetMoonTrans(const Math::matrix44 &trans)
{
	n_error("zhongdaohuan");//[zhongdaohuan][render_obj]临时删掉实现
	//m_pMoon->SetTransform(trans);
}

inline const Math::matrix44& MeshManager::GetMoonTrans() const
{
	n_error("zhongdaohuan");//[zhongdaohuan][render_obj]临时删掉实现
	//return m_pMoon->GetTransform();
}

}