/****************************************************************************
Copyright (c) 2011-2013,WebJet Business Division,CYOU

http://www.genesis-3d.com.cn

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#if defined (__USE_PHYSX__) || defined (__GENESIS_EDITOR__)
#include "stdneb.h"
#include "appframework/actor.h"
#include "PhysicsCapsuleShape.h"
#include "PhysicsMaterial.h"
#include "PhysicsServer.h"
#include "PhysicsUtil.h"
#include "PxPhysics.h"
#include "PxShape.h"
#include "PxRigidActor.h"
#include "PxCapsuleGeometry.h"
#include "physXfeature/PhysicsBodyComponent.h"
using namespace App;
namespace App
{
	__ImplementClass(App::PhysicsCapsuleShape, 'PHCS', App::PhysicsShape);


	PhysicsCapsuleShape::PhysicsCapsuleShape()
		:m_Height(1.0f)
		,m_Radius(0.5f)
		,m_Direct(1)
		,m_ActorScale(1.f,1.f,1.f)
	{
		m_ShapeType = CAPSULESHAPE;
	}

	PhysicsCapsuleShape::~PhysicsCapsuleShape()
	{
		OnDestory();
	}

	bool PhysicsCapsuleShape::OnCreate(GPtr<PhysicsBodyComponent> component)
	{
		m_ActorScale = component->GetActor()->GetWorldScale();
		if (m_bInheritActor)
		{
			OnResize(component->GetActor()->GetLocalBoundingBox());
			m_bInheritActor = false;
		}
		m_pGeometry = new PxCapsuleGeometry(_GetRealRadius(),_GetRealHeight()/2);
		n_assert(m_pGeometry)

			return true;
	}

	void PhysicsCapsuleShape::OnResize(const Math::bbox& bb)
	{
		if (!Math::n_nearequal(bb.size().length(),0.0f,0.001f))
		{
			Math::float2 _extent = _GetScaledLocalExtent(bb.size(), (Math::scalar)m_Direct);
			m_Radius = _extent.x();
			m_Height = _extent.y();
		}
	}
	//------------------------------------------------------------------------
	void PhysicsCapsuleShape::SetHeight( Math::scalar height )
	{
		m_Height = Math::n_abs(height);
		SetHeightDo(_GetRealHeight());
	}	
	//------------------------------------------------------------------------
	void PhysicsCapsuleShape::SetHeightDo(Math::scalar height)
	{
		if (m_pGeometry && m_pGeometry->getType() == PxGeometryType::eCAPSULE)
		{
			PxCapsuleGeometry* pCapsule = static_cast<PxCapsuleGeometry*>(m_pGeometry);
			n_assert(pCapsule);
			pCapsule->halfHeight = height/2 ;
			if (m_pShape && m_pShape->getGeometryType() == m_pGeometry->getType())
			{
				m_pShape->setGeometry(*pCapsule);
				m_bMassUpdate = true;
			}
		}
	}
	//------------------------------------------------------------------------
	void PhysicsCapsuleShape::SetRadius( Math::scalar radius )
	{
		m_Radius = Math::n_abs(radius);
		SetRadiusDo( _GetRealRadius() );
	}
	//------------------------------------------------------------------------
	void  PhysicsCapsuleShape::SetRadiusDo(Math::scalar radius)
	{
		if (m_pGeometry && m_pGeometry->getType() == PxGeometryType::eCAPSULE)
		{
			PxCapsuleGeometry* pCapsule = static_cast<PxCapsuleGeometry*>(m_pGeometry);
			n_assert(pCapsule);
			pCapsule->radius = radius ;
			if (m_pShape && m_pShape->getGeometryType() == m_pGeometry->getType())
			{
				m_pShape->setGeometry(*pCapsule);
				m_bMassUpdate = true;
			}
		}
	}	

	//-------------------------------------------------------------------------
	void PhysicsCapsuleShape::SetDirection(int dir)
	{
		if (m_Direct == dir)
		{
			return;
		}
		m_Direct = dir;
	}

	void PhysicsCapsuleShape::UpdateShapePosition()
	{
		if (m_pShape)
		{
			Math::quaternion _realRotation = Math::quaternion::multiply(m_Rotation, _GetQuatByDirection((Math::scalar)m_Direct));
			Math::float3 bbCenter(0.0f, 0.0f, 0.0f);
			if (m_pEntity && m_pEntity->GetBodyCom() && m_pEntity->GetBodyCom()->GetActor())
			{
				const Math::bbox& bb = m_pEntity->GetBodyCom()->GetActor()->GetLocalBoundingBox();
				bbCenter.set(bb.center().x(), bb.center().y(), bb.center().z());
			}
			PxMat44 pos = RotPosToPxMat(m_Center+bbCenter,_realRotation);
			m_pShape->setLocalPose(PxTransform(pos));
		}
	}

	//------------------------------------------------------------------------
	Math::quaternion PhysicsCapsuleShape::_GetQuatByDirection(Math::scalar dir)
	{
		Math::quaternion rotTemp;

		if (dir == 0)
		{//x
			// The X-Axis is the default direction, so do nothing.
		}
		else if (dir == 2)
		{//z
			Math::vector f4Axis(0.f,1.f,0.f);
			Math::scalar sRadian = (Math::scalar)(0.5f*PI);
			rotTemp = Math::quaternion::rotationaxis(f4Axis,sRadian);
		}
		else
		{//y
			Math::vector f4Axis(0.f,0.f,1.f);
			Math::scalar sRadian = (Math::scalar)(0.5f*PI);
			rotTemp = Math::quaternion::rotationaxis(f4Axis,sRadian);
		}

		return rotTemp;
	}

	//------------------------------------------------------------------------
	Math::float2 PhysicsCapsuleShape::_GetScaledLocalExtent(const Math::vector dimension, Math::scalar dir)
	{
		Math::scalar absoluteHeight = 0.01f;
		Math::scalar absoluteRadius = 0.01f;
		Math::vector _dimension(Math::n_abs(dimension.x()), Math::n_abs(dimension.y()), Math::n_abs(dimension.z()));
		if ( dir == 0 )
		{
			absoluteRadius = Math::n_max(_dimension.y(), _dimension.z()) * 0.5f;
			absoluteHeight = _dimension.x();
		}
		else if ( dir == 1 )
		{
			absoluteRadius = Math::n_max(_dimension.x(), _dimension.z()) * 0.5f;
			absoluteHeight = _dimension.y();
		}
		else
		{
			absoluteRadius = Math::n_max(_dimension.y(), _dimension.x()) * 0.5f;
			absoluteHeight = _dimension.z();
		}

		absoluteHeight	= Math::n_max (absoluteHeight, 0.01f);
		absoluteRadius	= Math::n_max (absoluteRadius, 0.01f);

		return Math::float2 (absoluteRadius, absoluteHeight);
	}

	//------------------------------------------------------------------------
	void PhysicsCapsuleShape::CreateRenderShape()
	{
	}

	//------------------------------------------------------------------------
	bool PhysicsCapsuleShape::CopyFrom( const GPtr<PhysicsShape>& pShape )
	{
		if ( !pShape.isvalid() || pShape->GetType() != m_ShapeType )
		{
			return false;
		}
		Super::CopyFrom(pShape);
		SetRadius(pShape.downcast<PhysicsCapsuleShape>()->m_Radius);
		SetHeight(pShape.downcast<PhysicsCapsuleShape>()->m_Height);
		SetDirection(pShape.downcast<PhysicsCapsuleShape>()->m_Direct);
		return true;
	}

	Math::scalar PhysicsCapsuleShape::CaculateVolume()
	{
		Math::scalar r = _GetRealRadius();
		Math::scalar h = _GetRealHeight();
		return r*r*r*(Math::scalar)PI*4.0f/3.0f+r*r*(Math::scalar)PI*h;
	}

	void PhysicsCapsuleShape::ScaleWithActor(const Math::vector& scale)
	{
		m_ActorScale = scale;
		SetRadiusDo(_GetRealRadius());
		SetHeightDo(_GetRealHeight());
		if(m_pEntity)
			m_pEntity->OnScaleWithActor();
	}

	Math::scalar PhysicsCapsuleShape::_GetRealHeight()
	{
		float _scale = 1.0f;
		if ( m_Direct == 0 )
		{
			_scale = m_ActorScale.x();
		}
		else if ( m_Direct == 1 )
		{
			_scale = m_ActorScale.y();
		}
		else
		{
			_scale = m_ActorScale.z();
		}
		return Math::n_max(Math::n_abs(m_Height * _scale), 0.01f);
// 		Math::scalar tempHeight = m_Height * _scale;
// 		Math::scalar realRadius = _GetRealRadius();
// 		return Math::n_max(tempHeight - 2 * realRadius, 0.01f);
	}

	Math::scalar PhysicsCapsuleShape::_GetRealRadius()
	{
		float _scale = 1.0f;
		float _actorX = Math::n_abs(m_ActorScale.x());
		float _actorY = Math::n_abs(m_ActorScale.y());
		float _actorZ = Math::n_abs(m_ActorScale.z());
		if ( m_Direct == 0 )
		{
			_scale = Math::n_max(_actorY, _actorZ);
		}
		else if ( m_Direct == 1 )
		{
			_scale = Math::n_max(_actorX, _actorZ);
		}
		else
		{
			_scale = Math::n_max(_actorY, _actorX);
		}
		return Math::n_max(Math::n_abs(m_Radius * _scale), 0.01f);
	}

	void PhysicsCapsuleShape::Save( AppWriter* pSerialize )
	{
		Super::Save(pSerialize);
		pSerialize->SerializeFloat("Radius", m_Radius);
		pSerialize->SerializeFloat("Height", m_Height);
		pSerialize->SerializeInt("Direction", m_Direct);
	}

	void PhysicsCapsuleShape::Load( Version ver, AppReader* pReader )
	{
		Super::Load(ver,pReader);
		pReader->SerializeFloat("Radius", m_Radius);
		pReader->SerializeFloat("Height", m_Height);
		pReader->SerializeInt("Direction", m_Direct);
	}

	bool PhysicsCapsuleShape::IsGeometryValid()
	{
		return IsValid() && ((PxCapsuleGeometry*)m_pGeometry)->isValid();
	}

}//end namespace app

#endif

