/** \file gyrostat.h
* Copyright (c) 2011,ChangYou.Webjet
* All rights reserved.
* Date         Ver    author          Comment
* 2011:10:26   1.0    ¿Ó±Û   
*/
#ifndef __GYROSTAT_H__
#define __GYROSTAT_H__

#include "foundation/math/float3.h"
#include "resource/meshres.h"

namespace Sample
{	

	using namespace Math;



	class Gyrostat
	{

	public:
		enum OPSTATE
		{
			OSTrans		= 0,
			OSRotate	= 1,
			OSScale		= 2,

		};

		enum OPMODE
		{
			OMGLOBAL	= 0,
			OMLOCAL		= 1,
			OMSCREEN	= 2,
		};

		struct GyrostatProp
		{
			Ptr<App::Actor> _actor;
			Math::float3	_axis;
			Math::float3	_oriVec;
			Math::float4	_color;
		};

	public: 
		Gyrostat();
		virtual ~Gyrostat();
		void	SetParent(const Ptr<App::Actor>& actor);
		void	SetSelectActor(const Ptr<App::Actor>& actor);
		void	UpdateActors();
		int		IsSelected();
		void	MouseChanged(const float2& oldPos, const float2& nowPos);
		void	SetOpState(OPSTATE op);
		void	SetOpMode(OPMODE op);

		static void	CreateCircleActor(GyrostatProp* gp, const Util::String& actName ,Resources::PositionData& position);
		static Ptr<App::Actor> CreateGyrostatActor(OPSTATE os, const Util::String& actName, Math::float3 vec, uint layerId, GyrostatProp* gp = NULL);

		


	private:
		void Create();
		Math::float3 GetScaleParent(const Ptr<App::Actor>& parentAct, Math::scalar& scale) const;
		void UpdateSelectActor(const int i);
		bool CalAxisWithQuat( const Math::quaternion& qut,Math::float4& out );


	private:

		Util::Array< GyrostatProp >	mGyrostatActors;
		Ptr<App::Actor>				mParentActor;
		int							mSelectIndex;
		OPSTATE						mOpState;
		OPMODE						mOpMode;


	};
	//---------------------------------------------------------------------------
	inline	void
		Gyrostat::SetParent(const Ptr<App::Actor>& actor)
	{
		mParentActor = actor;
		if(!mParentActor.isvalid())
			mSelectIndex = -1;
	}

	//----------------------------------------------------------------------------
	inline	int 
		Gyrostat::IsSelected()
	{
		return (mSelectIndex>=0);
	}

}
#endif