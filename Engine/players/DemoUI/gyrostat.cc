#include "stdneb.h"
#include "appframework/actor.h"
#include "graphicfeature/graphicsfeature.h"
#include "gyrostat.h"
#include "apputil/manuresutil.h"
#include "graphicfeature/components/meshcomponent.h"
#include "graphicfeature/components/meshrendercomponent.h"
#include "graphicfeature/components/cameracomponent.h"
#include "appframework/actormanager.h"
#include "math/polar.h"
#include "math/matrix44.h"
#include "math/ray.h"
#include "math/vector.h"
#include "graphicsystem/Renderable/Renderable.h"
#include "graphicsystem/Renderable/SimpleShapeRenderable.h"
#include "apputil/mouserayutil.h"
#include "apputil/intersectutil.h"

namespace Sample
{
using namespace Graphic;
using namespace App;
using namespace AppUtil;


Gyrostat::Gyrostat():mSelectIndex(-1)
{	
	mOpState = OSTrans;
	mOpMode  = OMGLOBAL;
	Create();	

}
//---------------------------------------------------------------------------
Gyrostat::~Gyrostat()
{
	//
}
//---------------------------------------------------------------------------
void
Gyrostat::CreateCircleActor(GyrostatProp* gp, const Util::String& actName ,Resources::PositionData& position)
{
	if(gp == NULL)
		return;

	for (int j = 0;j<=36;j++)
	{
		//Math::float3 tmp = quat * mGyrostatActors[i] ;
		Math::matrix44 mat = Math::matrix44::identity() ;
		mat = matrix44::rotationaxis( float4(gp->_axis.x(),
			gp->_axis.y(),
			gp->_axis.z(),
			1.0),
			j * 10 / 57.3f ) ;


		Math::float4 tmp(gp->_oriVec.x(),gp->_oriVec.y(),gp->_oriVec.z(),0) ;
		tmp = Math::matrix44::transform( mat,tmp );
		//Math::float3(quat.x(),quat.y(),quat.z()) * gp._oriVec ;

		position.Append(Math::float3(tmp.x(),tmp.y(),tmp.z())	);

	}


	Math::Color32 color(ubyte(gp->_color.x())*255,
		ubyte(gp->_color.y())*255,
		ubyte(gp->_color.z())*255,
		255);

	Ptr<Resources::MeshRes> mesh = AppUtil::ManuResUtil::CreateManuMesh_LineList( "Line_mesh", position.Size(),	&position[0],color);
	n_assert( mesh.isvalid() );

	Ptr<MeshComponent> pMeshComponent = MeshComponent::Create();
	pMeshComponent->SetMeshID( mesh->GetResourceId() );

	Ptr<MeshRenderComponent> pMeshRenderComponent = MeshRenderComponent::Create();		
	pMeshRenderComponent->SetShaderID(0, "shd:gyrostat.wjshader");

	pMeshRenderComponent->SetShaderConstantParam(0,"_calAngle",float4(1,1,1,1));
	gp->_actor->AttachComponent( pMeshComponent.upcast<Component>() );
	gp->_actor->AttachComponent( pMeshRenderComponent.upcast<Component>() );
}

//-----------------------------------------------------------------------------
Ptr<App::Actor> 
Gyrostat::CreateGyrostatActor(const OPSTATE os, const Util::String& actName, Math::float3 vec, uint layerId ,GyrostatProp* gp)
{
	Ptr<Actor> pActTaper = Actor::Create();
	pActTaper->SetName(actName);
	pActTaper->SetLayerID(layerId);
	Resources::PositionData position;
	Resources::Index16Container indicies;
	Util::String strResID; 
	if(os == OSTrans )
	{
		Math::scalar sThick = 0.05f;
		AppUtil::ManuResUtil::Build_Cone(vec,position,indicies,sThick);
		AppUtil::ManuResUtil::Build_Column(vec*0.9,position,indicies,sThick*0.1f);
		strResID =  "Cone_mesh";
	}
	else if(os == OSScale )
	{
		const Math::bbox localBB(Math::point(vec.x(),vec.y(),vec.z()), vector(0.05,0.05,0.05));
		AppUtil::ManuResUtil::Build_Box(localBB,position,indicies);
		AppUtil::ManuResUtil::Build_Column(vec,position,indicies,0.005f);
		strResID =  "box_mesh";
	}
	else if(os == OSRotate )
	{
		if(gp== NULL)
			return Ptr<Actor>();

		gp->_actor = pActTaper;
		CreateCircleActor(gp,actName,position);
		return pActTaper;
	}



	//----
	if (vec.x()<0.01 && vec.y()<0.01 && vec.z()<0.01)
		vec = float3(1,1,1);

	Math::Color32 color(ubyte(vec.x())*255,
		ubyte(vec.y())*255,
		ubyte(vec.z())*255,
		255);

	//CreateManuMesh_WithTopology
	Ptr<Resources::MeshRes> mesh = AppUtil::ManuResUtil::CreateManuMesh_WithTopology( strResID, position.Size(),	&position[0],color,indicies.Size(),&indicies[0]);
	n_assert( mesh.isvalid() );

	Ptr<MeshComponent> pMeshComponent = MeshComponent::Create();
	pMeshComponent->SetMeshID( mesh->GetResourceId() );

	Ptr<MeshRenderComponent> pMeshRenderComponent = MeshRenderComponent::Create();	
	pMeshRenderComponent->SetShaderID(0, "shd:gyrostat.wjshader");		
	pMeshRenderComponent->SetShaderConstantParam(0,"_calAngle",float4(1,0,0,0));

	pActTaper->AttachComponent( pMeshComponent.upcast<Component>() );
	pActTaper->AttachComponent( pMeshRenderComponent.upcast<Component>() );
	pActTaper->Active();

	if(!pActTaper.isvalid())
		return NULL;

	return pActTaper;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void
Gyrostat::Create()
{
	mGyrostatActors.Clear();

	

	//create three axis arrows
	{
		//
		//add transfor line
		Ptr<Actor> pActTransX = CreateGyrostatActor(OSTrans ,"AxisX", float3(1,0,0),6);		
		GyrostatProp gpTransX;
		gpTransX._axis = float3(1,0,0);
		gpTransX._oriVec = float3(0,0,0);
		gpTransX._color = float4(1,0,0,1);
		gpTransX._actor = pActTransX;
		mGyrostatActors.Append(gpTransX);

		Ptr<Actor> pActTransY = CreateGyrostatActor(OSTrans ,"AxisY", float3(0,1,0),6);	
		GyrostatProp gpTransY;
		gpTransY._axis = float3(0,1,0);
		gpTransY._oriVec = float3(0,0,0);
		gpTransY._color = float4(0,1,0,1);
		gpTransY._actor = pActTransY;
		mGyrostatActors.Append(gpTransY);

		Ptr<Actor> pActTransZ = CreateGyrostatActor(OSTrans ,"AxisZ", float3(0,0,1),6);
		GyrostatProp gpTransZ;
		gpTransZ._axis = float3(0,0,1);
		gpTransZ._oriVec = float3(0,0,0);
		gpTransZ._color = float4(0,0,1,1);
		gpTransZ._actor = pActTransZ;
		mGyrostatActors.Append(gpTransZ);
	}

	{
		//
		//add 3 rotate circle actors
		GyrostatProp gpRotX;
		gpRotX._axis = float3(1,0,0);
		gpRotX._oriVec = float3(0,1,0);
		gpRotX._color = float4(1,0,0,1);
		Ptr<Actor> CircleX = CreateGyrostatActor(OSRotate ,"CircleX", float3(1,0,0),6, &gpRotX);
		mGyrostatActors.Append(gpRotX);

		GyrostatProp gpRotY;
		gpRotY._axis = float3(0,1,0);
		gpRotY._oriVec = float3(0,0,1);
		gpRotY._color = float4(0,1,0,1);
		Ptr<Actor> CircleY = CreateGyrostatActor(OSRotate ,"CircleY", float3(0,1,0),6,&gpRotY);
		mGyrostatActors.Append(gpRotY);

		GyrostatProp gpRotZ;
		gpRotZ._axis = float3(0,0,1);
		gpRotZ._oriVec = float3(1,0,0);
		gpRotZ._color = float4(0,0,1,1);
		Ptr<Actor> CircleZ = CreateGyrostatActor(OSRotate ,"CircleZ", float3(0,0,1),6,&gpRotZ);
		mGyrostatActors.Append(gpRotZ);
	}

	{
		//
		//add 3 scale box actors
		Ptr<Actor> BoxX = CreateGyrostatActor( OSScale ,"BoxX", float3(1,0,0),6);
		GyrostatProp gpScaleX;
		gpScaleX._axis = float3(1,0,0);
		gpScaleX._oriVec = float3(0,0,0);
		gpScaleX._color = float4(1,0,0,1);
		gpScaleX._actor = BoxX;
		mGyrostatActors.Append(gpScaleX);


		Ptr<Actor> BoxY = CreateGyrostatActor( OSScale ,"BoxY", float3(0,1,0),6);
		GyrostatProp gpScaleY;
		gpScaleY._axis = float3(0,1,0);
		gpScaleY._oriVec = float3(0,0,0);
		gpScaleY._color = float4(0,1,0,1);
		gpScaleY._actor = BoxY;
		mGyrostatActors.Append(gpScaleY);


		Ptr<Actor> BoxZ = CreateGyrostatActor( OSScale ,"BoxZ", float3(0,0,1),6);
		GyrostatProp gpScaleZ;
		gpScaleZ._axis = float3(0,0,1);
		gpScaleZ._oriVec = float3(0,0,0);
		gpScaleZ._color = float4(0,0,1,1);
		gpScaleZ._actor = BoxZ;
		mGyrostatActors.Append(gpScaleZ);

		Ptr<Actor> BoxCenter = CreateGyrostatActor( OSScale ,"BoxCenter", float3(0,0,0),6);
		GyrostatProp gpScaleCenter;
		gpScaleCenter._axis = float3(1,1,1);
		gpScaleCenter._color = float4(1,1,1,1);
		gpScaleCenter._actor = BoxCenter;
		mGyrostatActors.Append(gpScaleCenter);
		//{
		//	//TODO: set boxCenter color
		//	BoxCenter->SetScale(Math::vector(0.1,0.1,0.1));
		//	Ptr<MeshRenderComponent> pMeshRenderComponent = BoxCenter->FindComponent( App::MeshRenderComponent::RTTI ).downcast<App::MeshRenderComponent>() ;
		//	pMeshRenderComponent->SetShaderID(0, "shd:gyrostat.wjshader");
		//	pMeshRenderComponent->SetShaderConstantParam(0,"_outColor",gpScaleCenter._color);//_outColor
		//}
	}

	////set actor
	//for (int i = 3;i < mGyrostatActors.Size() ; i++)
	//{
	//	Resources::PositionData position;

	//	for (int j = 0;j<=36;j++)
	//	{
	//		//Math::float3 tmp = quat * mGyrostatActors[i] ;
	//		Math::matrix44 mat = Math::matrix44::identity() ;
	//		mat = matrix44::rotationaxis( float4(mGyrostatActors[i]._axis.x(),
	//			mGyrostatActors[i]._axis.y(),
	//			mGyrostatActors[i]._axis.z(),
	//			1.0),
	//			j * 10 / 57.3f ) ;


	//		Math::float4 tmp(mGyrostatActors[i]._oriVec.x(),mGyrostatActors[i]._oriVec.y(),mGyrostatActors[i]._oriVec.z(),0) ;
	//		tmp = Math::matrix44::transform( tmp ,mat);
	//		//Math::float3(quat.x(),quat.y(),quat.z()) * mGyrostatActors[i]._oriVec ;

	//		position.Append(Math::float3(tmp.x(),tmp.y(),tmp.z())	);

	//	}


	//	Math::Color32 color(ubyte(mGyrostatActors[i]._color.x())*255,
	//		ubyte(mGyrostatActors[i]._color.y())*255,
	//		ubyte(mGyrostatActors[i]._color.z())*255,
	//		255);

	//	Ptr<Resources::MeshRes> mesh = AppUtil::ManuResUtil::CreateManuMesh_LineList( "Line_mesh", position.Size(),	&position[0],color);
	//	n_assert( mesh.isvalid() );

	//	Ptr<MeshComponent> pMeshComponent = MeshComponent::Create();
	//	pMeshComponent->SetMeshID( mesh->GetResourceId() );

	//	Ptr<MeshRenderComponent> pMeshRenderComponent = MeshRenderComponent::Create();		
	//	pMeshRenderComponent->SetShaderID(0, "shd:gyrostat.wjshader");

	//	pMeshRenderComponent->SetShaderConstantParam(0,"_calAngle",float4(1,1,1,1));
	//	mGyrostatActors[i]._actor->AttachComponent( pMeshComponent.upcast<Component>() );
	//	mGyrostatActors[i]._actor->AttachComponent( pMeshRenderComponent.upcast<Component>() );
	//}
}
//------------------------------------------------------------------------------
//void
//Gyrostat::SetOpState(OPSTATE op)
//{
//	mOpState = op;
//}
//---------------------------------------------------------------------------
void 
Gyrostat::UpdateActors()
{
	if(mGyrostatActors.IsEmpty())
		Create();
	
	if(!mParentActor.isvalid())
	{
		for (int i = 0;i < mGyrostatActors.Size() ; i++)
		{
			mGyrostatActors[i]._actor->Deactive();
		}
		return;
	}
	
	scalar scale;
	float3 parPos = GetScaleParent(mParentActor,scale);

	if( mOpState == OSScale )
	{
		int nindex = mGyrostatActors.Size()-1;
		//TODO:: update center scale Box
		if (!mGyrostatActors[nindex]._actor->IsActive())
			mGyrostatActors[nindex]._actor->Active();
		mGyrostatActors[nindex]._actor->SetPosition(vector(parPos.x(),parPos.y(),parPos.z()));
		mGyrostatActors[nindex]._actor->SetScale(vector(scale,scale,scale)* 1.5f);
		UpdateSelectActor(nindex);

	}

	int uType =  (int)mOpState;//mOpState & 0x01;
	for (int i = uType * 3;i < uType * 3 + 3 ; i++)
	{
		if (mGyrostatActors[i]._actor.isvalid() ) 
		{
			if (!mGyrostatActors[i]._actor->IsActive())
				mGyrostatActors[i]._actor->Active();

			//update
			mGyrostatActors[i]._actor->SetPosition(vector(parPos.x(),parPos.y(),parPos.z()));
			mGyrostatActors[i]._actor->SetScale(vector(scale,scale,scale));

			Math::quaternion qRot;
			if (mOpMode == OMLOCAL || mOpState == OSScale)
				qRot = mParentActor->GetWorldRotation();
			else if (mOpMode == OMGLOBAL)
				qRot = quaternion(mGyrostatActors[i]._axis.x(),
					mGyrostatActors[i]._axis.y(),
					mGyrostatActors[i]._axis.z(),
					1);
			else if (mOpMode == OMSCREEN)
			{
				const Ptr<Actor>& pActorCam = GraphicsFeature::Instance()->GetDefaultCameraActor();
				if (!pActorCam.isvalid())
					return;
				qRot = pActorCam->GetRotation();
			}

			qRot = quaternion::normalize(qRot);
			mGyrostatActors[i]._actor->SetRotation(qRot);
			UpdateSelectActor(i);	
		
		}		
	}
}

//---------------------------------------------------------------------------
void
Gyrostat::UpdateSelectActor(const int i)
{

	const Ptr<Actor>& pActCam = GraphicsFeature::Instance()->GetDefaultCameraActor();
	Ptr<MeshRenderComponent> pMeshRenderComponent = mGyrostatActors[i]._actor->FindComponent( App::MeshRenderComponent::RTTI ).downcast<App::MeshRenderComponent>() ;

	float4 campos = float4(pActCam->GetPosition().x(),pActCam->GetPosition().y(),pActCam->GetPosition().z(),1);
	float4 ccpos = float4(mGyrostatActors[i]._actor->GetPosition().x(),
		mGyrostatActors[i]._actor->GetPosition().y(),
		mGyrostatActors[i]._actor->GetPosition().z(),
		1);

	float4 clr = mGyrostatActors[i]._color;

	float4 fAngle;
	if(mSelectIndex == i)
	{
		clr = float4(1,1,0,1);
		fAngle = float4(0,0,0,0);
	}
	else
	{
		if (mOpState == OSRotate)
			fAngle = float4(1,1,0,0);
		else
			fAngle = float4(1,0,0,0);
	}

	pMeshRenderComponent->SetShaderConstantParam(0,"_calAngle",fAngle);
	pMeshRenderComponent->SetShaderConstantParam(0,"_outColor",clr);//_outColor
	pMeshRenderComponent->SetShaderConstantParam(0,"_camPos",campos);
	pMeshRenderComponent->SetShaderConstantParam(0,"_circleCenter",ccpos);

}
//-------------------------------------------------------------------------------
Math::float3 
Gyrostat::GetScaleParent(const Ptr<App::Actor>& parentAct , Math::scalar& scale) const
{
	const Ptr<Actor>& pActorCam = GraphicsFeature::Instance()->GetDefaultCameraActor();
	if ( !pActorCam.isvalid() )
		return float3(0,0,0);
	
	Math::float3 campos(pActorCam->GetWorldPosition().x(),pActorCam->GetWorldPosition().y(),pActorCam->GetWorldPosition().z());
	Math::float3 parPos(parentAct->GetWorldPosition().x(),parentAct->GetWorldPosition().y(),parentAct->GetWorldPosition().z());
	Math::float3 normalizeDir(parPos.x() - campos.x(),
		parPos.y() - campos.y(),
		parPos.z() - campos.z());

	scale = normalizeDir.length()/10;
	return parPos;
}
void 
Gyrostat::SetSelectActor(const Ptr<App::Actor>& actor)
{
	for (int i = 0;i < mGyrostatActors.Size() ; i++)
	{
		if (actor == mGyrostatActors[i]._actor)
		{
			mSelectIndex = i;
			return ;
		}
	}
}
//---------------------------------------------------------------------------------------------
void	
Gyrostat::MouseChanged(const float2& oldPos, const float2& nowPos)
{

	float2 offsetPos(nowPos.x()-oldPos.x(),nowPos.y()-oldPos.y());


	//Math::Ray oldRay;
	//AppUtil::IntersectUtil::ComputeDefaultWorldMouseRay( oldPos, oldRay );
	//Math::Ray newRay;
	//AppUtil::IntersectUtil::ComputeDefaultWorldMouseRay( nowPos, newRay );

	switch( mOpState )
	{
	case OSTrans:	
		{
			//todo
			const Ptr<Actor>& pActorCam = GraphicsFeature::Instance()->GetDefaultCameraActor();
			if ( !pActorCam.isvalid() )
				return ;
			const Ptr<Component> obj = pActorCam->FindComponent( CameraComponent::RTTI );
			n_assert( obj.isvalid() );
			Ptr<CameraComponent> pCamera = obj.downcast<CameraComponent>();


			float nearWidth, nearHeight, nearPlaneDis, farPlaneDis;
			pCamera->GetProjParam( &nearWidth , &nearHeight , &nearPlaneDis , &farPlaneDis );

			vector parentvec(mParentActor->GetPosition().x(),
				mParentActor->GetPosition().y(),
				mParentActor->GetPosition().z());
			vector camvec(pActorCam->GetPosition().x(),
				pActorCam->GetPosition().y(),
				pActorCam->GetPosition().z());

			Math::scalar dis_actr_cam = Math::vector(parentvec-camvec).length();//角色到相机距离

			scalar resx = dis_actr_cam / nearPlaneDis * ( nearWidth * offsetPos.x() ) ;
			scalar resy = dis_actr_cam / nearPlaneDis * ( nearHeight * offsetPos.y() ) ;

			resy = -resy;

			Math::quaternion q = pActorCam->GetRotation();

			Math::vector camAxisX(1,0,0);
			Math::vector camAxisY(0,1,0);
			CalAxisWithQuat(q,camAxisX);
			CalAxisWithQuat(q,camAxisY);
			Math::vector dummyDrag = camAxisX * resx + camAxisY * resy;
	
			Math::vector selectAxis(mGyrostatActors[mSelectIndex]._axis.x(),
				mGyrostatActors[mSelectIndex]._axis.y(),
				mGyrostatActors[mSelectIndex]._axis.z());

			float xx = Math::vector::dot3( selectAxis , camAxisX ) ;
			float yy = Math::vector::dot3( selectAxis , camAxisY ) ;

			// axis on dummy screen 
			Math::vector axOnScrn = camAxisX * xx + camAxisY * yy ;

			// new manner
			float moveScale = Math::vector::dot3( axOnScrn , dummyDrag );

			if (mOpMode == OMLOCAL)
			{
				//if opMode == OMSCREEN then q = Camra->GetRotation();
				q = mParentActor->GetRotation();
				CalAxisWithQuat(q,selectAxis);
			}
			else if(mOpMode == OMSCREEN)
			{

				CalAxisWithQuat(q,selectAxis);
				//parAxis = Math::matrix44::transform(parAxis,mParentActor->GetTransform());
			}

			//TODO: calculate the angle from the vector(world axis )   
			//		to the other vector(cam Axis  or  local axis after rotated)

			Math::scalar retDot = selectAxis.dotProduct(Math::vector(mGyrostatActors[mSelectIndex]._axis.x(),
				mGyrostatActors[mSelectIndex]._axis.y(),
				mGyrostatActors[mSelectIndex]._axis.z()));

			//calc the vector on nearPlane
			//Math::float3 oldPoint = oldRay.PointAt(nearPlaneDis);
			//Math::float3 newPoint = newRay.PointAt(nearPlaneDis);
			//Math::vector disVec = Math::vector(newPoint.x()- oldPoint.x(),
			//	newPoint.y()- oldPoint.y(),
			//	newPoint.z()- oldPoint.z());
			//Math::scalar retDot = disVec.dotProduct(selectAxis);

			if(retDot<0.0)
				selectAxis =selectAxis * -1.0f;

			if( mOpMode != OMLOCAL && mParentActor->GetParent() )
			{
				Math::matrix44 parentInverseMat = Math::matrix44::inverse(mParentActor->GetParent()->GetWorldTransform());
				selectAxis = Math::vector::transform(parentInverseMat, selectAxis);
			}
			//if cam rotate to offside of  selectActor, x,y axis must set -1
			Math::vector newPos = parentvec + selectAxis * moveScale;

			mParentActor->SetPosition(newPos);

		}
		break;
	case OSRotate:		
		{
			if(!mParentActor.isvalid())
				return;
			Math::quaternion q;			

			Math::vector selectAxis(mGyrostatActors[mSelectIndex]._axis.x(),
				mGyrostatActors[mSelectIndex]._axis.y(),
				mGyrostatActors[mSelectIndex]._axis.z());	

			switch(mOpMode)
			{
			case OMGLOBAL:
				{
					q = mParentActor->GetRotation();
					if(mParentActor->GetParent())
					{
						Math::matrix44 parentInverseMat = Math::matrix44::inverse(mParentActor->GetParent()->GetWorldTransform());
						selectAxis = Math::vector::transform(parentInverseMat, selectAxis);
					}
				}
			case OMLOCAL:
				q = mParentActor->GetRotation();
				break;
			case OMSCREEN:
				{
					const Ptr<Actor>& pActorCam = GraphicsFeature::Instance()->GetDefaultCameraActor();
					if (!pActorCam.isvalid())
						return;
					q = pActorCam->GetRotation();

					Math::matrix44 parentInverseMat = Math::matrix44::inverse(pActorCam->GetWorldTransform());
					selectAxis = Math::vector::transform(parentInverseMat, selectAxis);
					//Math::vector(selectAxis.x(),selectAxis.z(),selectAxis.y())
				}
				break;
			}

			Math::float4 rotate(selectAxis.x(),	selectAxis.y(),	selectAxis.z(),	1);	

			if ( mOpMode != OMGLOBAL )
				CalAxisWithQuat(q,rotate);

			float rV = offsetPos.x() + offsetPos.y() ;
			rV *= 30;
			Math::quaternion rq = Math::quaternion::rotationaxis( rotate , rV );
			Math::quaternion finalQ = Math::quaternion::multiply(  rq, mParentActor->GetRotation() ) ;
			mParentActor->SetRotation( finalQ );

		}
		break;
	case OSScale:
		{
			vector scalVec = mParentActor->GetScale();
			float rV = offsetPos.x()+offsetPos.y() ;
				if(mGyrostatActors[mSelectIndex]._axis.x()>0.9)
					scalVec.set_x(scalVec.x()*(1+rV));
				if(mGyrostatActors[mSelectIndex]._axis.y()>0.9)
				{
					if(mSelectIndex == mGyrostatActors.Size()-1)
						scalVec.set_y(scalVec.y()*(1+rV));
					else
						scalVec.set_y(scalVec.y()*(1-rV));
				}
				if(mGyrostatActors[mSelectIndex]._axis.z()>0.9)
					scalVec.set_z(scalVec.z()*(1+rV));
	
			mParentActor->SetScale(scalVec);


		}
		break;
	}



}

//---------------------------------------------------------------------------
void
Gyrostat::SetOpState(OPSTATE op)
{
	mOpState = op;

	for (int i = 0;i < mGyrostatActors.Size() ; i++)
	{
		mGyrostatActors[i]._actor->Deactive();
	}
}
//----------------------------------------------------------------------------
void
Gyrostat::SetOpMode(OPMODE op)
{
	mOpMode = op;
}
//-------------------------------------------------------------------------------
bool 
Gyrostat::CalAxisWithQuat( const Math::quaternion& qut,Math::float4& out )
{

	Math::matrix44 rotMat = Math::matrix44::rotationquaternion( qut ) ;

	out = Math::float4::transform( rotMat, out );

	return true ;
}

//-------------------------------------------------------------------------


}