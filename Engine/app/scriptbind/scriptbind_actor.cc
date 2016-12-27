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
#include "stdneb.h"
#include "foundation/math/newMath/new_quaternion.h"
#include "foundation/core/rtti.h"
#include "graphicfeature/components/animationcomponent.h"
#include "graphicfeature/components/meshrendercomponent.h"
#include "particlefeature/components/particlerendercomponent.h"
#include "appframework/actormanager.h"
#include "appframework/app_fwd_decl.h"
#include "appframework/actor.h"
#include "basegamefeature/managers/sceneschedulemanager.h"
#include "scriptfeature/inc/script_component.h"
#include "scriptfeature/inc/script_utility.h"


/* SunHao 2012/03/05
	rules of making names
	file name:the same with class name
	registration function: ICallReg_namespace_classname
	internal call functions:ICall_classname_functionname
	
	rules of function turn
	the turn which function appears shall be the same as the class which it binds
*/
namespace App
{

	void ICallReg_ScriptRuntime_Actor( void );

	//------------------------------------------------------------------------
	static void ICall_Actor_Bind( MonoObject* pMonoObj )
	{
		Actor* pActor = Actor::Create();
		n_assert( NULL!=pActor );
        BindCppObjWithMonoObj<Actor>( pActor, pMonoObj );
	}
	//------------------------------------------------------------------------
	static void ICall_Actor_Release( MonoObject* pMonoObj )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		Actor* pActor = self.GetCppObjPtr();
		n_assert( NULL!=pActor );
		ReleaseCppObjWithMonoObj<Actor>( pActor, pMonoObj );
	}
	//--------------------------------basic-----------------------------------//
	//------------------------------------------------------------------------
	static void ICall_Actor_SetName( MonoObject* pMonoObj, MonoString* pMonoString )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );

		Util::String name = Utility_MonoStringToCppString( pMonoString );
		self->SetName( name );
		__Actor_Property_Changed(self.GetCppObjPtr());
	}
	//------------------------------------------------------------------------
	static MonoString* ICall_Actor_GetName( MonoObject* pMonoObj )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		Util::String sName = self->GetName();

		return Utility_NewMonoString( sName );
	}
	//------------------------------------------------------------------------
	static MonoString* ICall_Actor_GetTemplateName( MonoObject* pMonoObj )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		Util::StringAtom sNameAtom = self->GetTemplateName();
		Util::String sName( sNameAtom.Value() );

		return Utility_NewMonoString( sName );
	}
	//------------------------------------------------------------------------
	static void ICall_Actor_SetLayerID( MonoObject* pMonoObj, uint32 layerID )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		LayerID lID = static_cast<LayerID>( layerID );
		self->SetLayerID( lID );
		__Actor_Property_Changed(self.GetCppObjPtr());
	}
	//------------------------------------------------------------------------
	static uint32 ICall_Actor_GetLayerID( MonoObject* pMonoObj )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		uint layerID = self->GetLayerID();
		return static_cast<uint32>( layerID ); 
	}
	//------------------------------------------------------------------------
	static mono_bool ICall_Actor_IsLinkTemplate( MonoObject* pMonoObj )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		bool blink =  self->IsLinkTemplate();
		if ( blink )
		{
			return c_iMonoBool_True;
		}
		else
		{
			return c_iMonoBool_False;
		}
	}
	//------------------------------------------------------------------------
	static void ICall_Actor_SetLinkTemplate( MonoObject* pMonoObj, mono_bool blink )
	{
		ScriptObjWrapper<Actor> self( pMonoObj);
		bool b = false;
		if ( c_iMonoBool_True==blink )
		{
			b = true;
		}

		self->SetLinkTemplate( b );
		__Actor_Property_Changed(self.GetCppObjPtr());
	}	
	//------------------------------------------------------------------------
	static void ICall_Actor_SetTagID( MonoObject* pMonoObj, uint32 tagID )
	{
		ScriptObjWrapper<Actor> self( pMonoObj);
		self->SetTagID( tagID );
		__Actor_Property_Changed(self.GetCppObjPtr());
	}
	//------------------------------------------------------------------------
	static uint32 ICall_Actor_GetTagID( MonoObject* pMonoObj )
	{
		ScriptObjWrapper<Actor> self( pMonoObj);
		return self->GetTagID();
	}
	//------------------------------------------------------------------------
	static uint32 ICall_Actor_GetFastId( MonoObject* pMonoObj )
	{
		ScriptObjWrapper<Actor> self( pMonoObj);
		return self->GetFastId();
	}
	//------------------------------------------------------------------------
	static MonoArray* ICall_Actor_GetGuid( MonoObject* pMonoObj)
	{
		ScriptObjWrapper<Actor> self( pMonoObj);
		return Utility_UtilGuidToMonoByteArray(self->GetGUID());
	}
	//------------------------------------------------------------------------
	static mono_bool ICall_Actor_IsActive( MonoObject* pMonoObj )
	{
		ScriptObjWrapper<Actor> self( pMonoObj);
		return self->IsActive();		
	}

	//------------------------------------------------------------------------
	static mono_bool ICall_Actor_GetActiveControl( MonoObject* pMonoObj )
	{
		ScriptObjWrapper<Actor> self( pMonoObj);
		return self->GetActiveControl();		
	}
	//------------------------------------------------------------------------

	static void ICall_Actor_Active( MonoObject* pMonoObj, mono_bool forceActiveControlofChild )
	{
		ScriptObjWrapper<Actor> self( pMonoObj);

		bool bIncChild = true;
		if ( c_iMonoBool_False == forceActiveControlofChild  )
		{
			bIncChild = false;
		}

		self->Active( bIncChild );
		__Actor_Property_Changed(self.GetCppObjPtr());
	}
	//------------------------------------------------------------------------
	static void ICall_Actor_Deactive( MonoObject* pMonoObj, mono_bool forceActiveControlofChild )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		n_assert( NULL!=self.GetCppObjPtr() );
		

		TActorPtr pSelfPtr( self.GetCppObjPtr() );
		ActorManager::Instance()->DeactiveActorDelayed( pSelfPtr, c_iMonoBool_True == forceActiveControlofChild );
		__Actor_Property_Changed(self.GetCppObjPtr());
	}
	//--------------------------------Parent----------------------------------//	
	//------------------------------------------------------------------------
	static MonoObject* ICall_Actor_GetParent( MonoObject* pMonoObj )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		Actor* pParent =  self->GetParent();
		if ( NULL==pParent )
		{
			return NULL;
		}
		return CppObjectToScriptObj( *pParent );
	}
	//------------------------------------------------------------------------
	static void ICall_Actor_SetParent( MonoObject* pMonoObj, MonoObject* pParent )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		Actor* parent = ScriptObjToCppPointer<Actor>(pParent);
		self->SetParent( parent );
		SCRIPT_API_HOOK_MESSEGE(Actor,SetParent,(self.GetCppObjPtr(), parent));
	}
	//------------------------------------------------------------------------
	static SizeT ICall_Actor_GetChildCount( MonoObject* pMonoObj )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );

		return self->GetChildCount();
	}
	//------------------------------------------------------------------------	
	static MonoObject* ICall_Actor_GetChild( MonoObject* pMonoObj, IndexT index )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		const TActorPtr& pChild = self->GetChild(index);
		return CppPointerToScriptObj( pChild.get_unsafe() );
	}
	//------------------------------------------------------------------------
	static MonoObject* ICall_Actor_FindChild( MonoObject* pMonoObj, uint32 fastId )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		Actor::FastId id = static_cast<Actor::FastId>( fastId );
		TActorPtr pChild = self->FindChild( id );
		return CppPointerToScriptObj( pChild.get_unsafe() );
	}

	static MonoObject* ICall_Actor_FindChildByName(MonoObject* pMonoObj, MonoString* name)
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		Util::String v1;
		Utility_MonoStringToCppString(name, v1);
		TActorPtr pChild = self->FindChild( v1 );
		return CppPointerToScriptObj( pChild.get_unsafe() );
	}
	//------------------------------------------------------------------------
	static IndexT ICall_Actor_FindChildIndex( MonoObject* pMonoObj, uint32 fastId )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		Actor::FastId id = static_cast<Actor::FastId>( fastId );
		return self->FindChildIndex( id );
	}

	static IndexT ICall_Actor_FindChildIndexByName(MonoObject* pMonoObj, MonoString* name)
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		Util::String v1;
		Utility_MonoStringToCppString(name, v1);
		return self->FindChildIndex( v1 );
	}
	//------------------------------------------------------------------------
	static void ICall_Actor_AddChild( MonoObject* pMonoObj, MonoObject* pChildObj )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		Actor* child = ScriptObjToCppPointer<Actor>(pChildObj);
		GPtr<Actor> pChildPtr( child );
		self->AddChild( pChildPtr );
		SCRIPT_API_HOOK_MESSEGE(Actor, AddChild, (self.GetCppObjPtr(), child));
	}
	//------------------------------------------------------------------------
	static void ICall_Actor_RemoveChild( MonoObject* pMonoObj, IndexT idx )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		self->RemoveChild( idx );
		SCRIPT_API_HOOK_MESSEGE(Actor, RemoveChild, (self.GetCppObjPtr(), idx));
	}
	//----------------------------Transform -----------------------------------//
	//------------------------------------------------------------------------
	static void ICall_Actor_SetLocalPos( MonoObject* pMonoObj, Math::float3& pos )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		self->SetPosition( Utility_Float3ToVector(pos) );
		__Actor_Property_Changed(self.GetCppObjPtr());
	}
	//------------------------------------------------------------------------
	static void ICall_Actor_SetLocalRotation( MonoObject* pMonoObj, Math::quaternion& rotation )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		self->SetRotation( rotation );
		__Actor_Property_Changed(self.GetCppObjPtr());
	}
	//------------------------------------------------------------------------
	static void ICall_Actor_SetLocalScale( MonoObject* pMonoObj, Math::float3& scale )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		self->SetScale( Utility_Float3ToVector(scale) );
		__Actor_Property_Changed(self.GetCppObjPtr());
	}
	//------------------------------------------------------------------------
	static void ICall_Actor_GetLocalPos( MonoObject* pMonoObj, Math::float3& outPos )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		outPos = Utility_VectorToFloat3(self->GetPosition());
	}
	//------------------------------------------------------------------------
	static void ICall_Actor_GetLocalRotation( MonoObject* pMonoObj, Math::quaternion& outRotation )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		outRotation = self->GetRotation();
	}
	//------------------------------------------------------------------------
	static void ICall_Actor_GetLocalScale( MonoObject* pMonoObj, Math::float3& outScale )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		outScale = Utility_VectorToFloat3(self->GetScale());
	}
	//------------------------------------------------------------------------
	static void ICall_Actor_Roll( MonoObject* pMonoObj, float angle )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		self->Roll( angle );
		__Actor_Property_Changed(self.GetCppObjPtr());
	}
	//------------------------------------------------------------------------
	static void ICall_Actor_Pitch( MonoObject* pMonoObj,float angle )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		self->Pitch( angle );
		__Actor_Property_Changed(self.GetCppObjPtr());
	}
	//------------------------------------------------------------------------
	static void ICall_Actor_Yaw( MonoObject* pMonoObj,float angle )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		self->Yaw( angle );
		__Actor_Property_Changed(self.GetCppObjPtr());
	}
	//------------------------------------------------------------------------
	static void ICall_Actor_Rotate( MonoObject* pMonoObj, float roll, float picth, float yaw )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		self->Rotate(roll, picth, yaw);
		__Actor_Property_Changed(self.GetCppObjPtr());
	}
	//------------------------------------------------------------------------
	static void ICall_Actor_SetTransform( MonoObject* pMonoObj, Math::matrix44& matrix)
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		self->SetTransform( matrix );
		__Actor_Property_Changed(self.GetCppObjPtr());
	}
	//------------------------------------------------------------------------
	static void ICall_Actor_GetTransform( MonoObject* pMonoObj, Math::matrix44& outMatrix)
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		outMatrix = self->GetTransform();
	}
	//------------------------------------------------------------------------
	static void ICall_Actor_SetWorldTransform( MonoObject* pMonoObj, Math::matrix44& matrix)
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		self->SetWorldTransform(matrix);
		__Actor_Property_Changed(self.GetCppObjPtr());
	}
	//------------------------------------------------------------------------
	static void ICall_Actor_GetWorldTransform( MonoObject* pMonoObj, Math::matrix44& outMatrix)
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		outMatrix = self->GetWorldTransform();
	}
	//------------------------------------------------------------------------
	static void ICall_Actor_SetWorldRotation( MonoObject* pMonoObj, Math::quaternion& rotation )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		self->SetWorldRotation(rotation);
		__Actor_Property_Changed(self.GetCppObjPtr());
	}
	//------------------------------------------------------------------------
	static void ICall_Actor_SetWorldPos( MonoObject* pMonoObj, Math::float3& pos )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		self->SetWorldPosition(Utility_Float3ToVector(pos));
		__Actor_Property_Changed(self.GetCppObjPtr());
	}
	//------------------------------------------------------------------------
	static void ICall_Actor_SetWorldScale( MonoObject* pMonoObj, Math::float3& scale )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		self->SetWorldScale(Utility_Float3ToVector(scale));
		__Actor_Property_Changed(self.GetCppObjPtr());
	}
	//------------------------------------------------------------------------
	static void ICall_Actor_GetWorldRotation( MonoObject* pMonoObj, Math::quaternion& outRotation )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		outRotation = self->GetWorldRotation();
	}
	//------------------------------------------------------------------------
	static void ICall_Actor_GetWorldPos( MonoObject* pMonoObj, Math::float3& outPos )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		outPos = Utility_VectorToFloat3(self->GetWorldPosition());
	}
	//------------------------------------------------------------------------
	static void ICall_Actor_GetWorldScale( MonoObject* pMonoObj, Math::float3& outScale )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		outScale = Utility_VectorToFloat3(self->GetWorldScale());
	}
	//-------------------------------boundingbox------------------------------//
	//------------------------------------------------------------------------
	static void ICall_Actor_GetWorldBoundingBox( MonoObject* pMonoObj, bbox_script& outBbox )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		outBbox = self->GetWorldBoundingBox();
	}
	//------------------------------------------------------------------------
	static void ICall_Actor_GetWorldBoundingBoxWithChild( MonoObject* pMonoObj, bbox_script& outBbox )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		outBbox = self->GetWorldBoundingBoxWithChild();
	}
	//------------------------------------------------------------------------
	static void ICall_Actor_GetLocalBoundingBox( MonoObject* pMonoObj, bbox_script& outBbox )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		outBbox = self->GetLocalBoundingBox();
	}
	//------------------------------------------------------------------------
	static void ICall_Actor_SetLocalBoundingBox( MonoObject* pMonoObj, bbox_script& bbox )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		self->SetLocalBoundingBox( bbox.toBbox() );
		__Actor_Property_Changed(self.GetCppObjPtr());
	}
	//-------------------------------component--------------------------------//
	//------------------------------------------------------------------------
	static void ICall_Actor_RemoveComponent( MonoObject* pMonoObj, MonoObject* pPEC ) 
	{ 
		if(pPEC == NULL)
			return;
		ScriptObjWrapper<Actor> self( pMonoObj ); 

		Component* com = ScriptObjToCppPointer<Component>(pPEC);
		GPtr<Component> pCom = static_cast<Component*>( com ); 
		self->RemoveComponent( pCom ); 
		__Actor_Property_Changed(self.GetCppObjPtr());
	}
	//------------------------------------------------------------------------
	static MonoObject* ICall_Actor_AddComponentByName( MonoObject* pMonoObj, MonoString* pMonoString )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		Util::String sName = Utility_MonoStringToCppString( pMonoString );
		Component* pCom = AddComponentByName( self.GetCppObjPtr(), sName );
		if ( NULL==pCom )
		{
			return NULL;
		}
		__Actor_Property_Changed(self.GetCppObjPtr());
		return CppObjectToScriptObj( *pCom );
	}
	//------------------------------------------------------------------------
	static MonoObject* ICall_Actor_GetComponentByName( MonoObject* pMonoObj, MonoString* pMonoString )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		Util::String sName = Utility_MonoStringToCppString( pMonoString );		
		Component* pCom = GetComponentByName( self.GetCppObjPtr(), sName );
		if ( NULL==pCom )
		{
			return NULL;
		}

		return CppObjectToScriptObj( *pCom );
	}
	//------------------------------------------------------------------------
	static mono_bool ICall_Actor_IsAllResourcePrepared( MonoObject* pMonoObj )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );

		return Utility_MonoBool( self->IsAllResourcePrepared() );
	}

	static void ICall_Actor_SetPriority( MonoObject* pMonoObj ,Resources::Priority priority)
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		self->SetPriority(priority);
	}

	static int ICall_Actor_GetPriority( MonoObject* pMonoObj )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		return self->GetPriority();
	}

	static int ICall_Actor_GetRefCount( MonoObject* pMonoObj )
	{
		ScriptObjWrapper<Actor> self( pMonoObj );
		return self->GetRefCount();
	}
}



#include "autogen/scriptbind_Actor_register.h"
