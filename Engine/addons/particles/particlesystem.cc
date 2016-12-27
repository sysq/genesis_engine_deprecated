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
#include "particles/particlesystem.h"
#include "particles/particletarget.h"
#include "particles/particle.h"
#include "particles/particleserver.h"
#include "app/basegamefeature/managers/timemanager.h"
#include "particles/particlepool.h"
#include "particles/emitters/particleModelEmitter.h"
#include "particles/emitters/particleBoxEmitter.h"
#include "particles/emitters/particleConeEmitter.h"
#include "particles/emitters/particleSphereSurfaceEmitter.h"
#include "particles/targets/particlebillboardtarget.h"
#include "particles/targets/particleRibbonTrailTarget.h"
#include "particles/targets/particleGPUTarget.h"

namespace Particles
{
	__ImplementClass(Particles::ParticleSystem,'PARS', Core::RefCounted)

		const GPtr<ParticleSystem> ParticleSystem::NullParSystem(NULL);
	const Timing::Time ParticleSystem::DefaultFrameTime(1.0/30);
	const Timing::Time ParticleSystem::MinFrameTime(0.000001);
	const float		   ParticleSystem::MinUpdateTime(0.033f);
	const float		   ParticleSystem::MaxUpdateTime(0.33f);
	const float		   ParticleSystem::UpdateFactor(0.002f);
	//------------------------------------------------------------------------
	ParticleSystem::ParticleSystem()
		: mIsActive(false)
		, mIsPreLoop(false)
		, mPlayBackTime(0.0)
		, mTransparency(255)
		, mCurFrameTime(0.0)
		, mFrameIndex(0)
		, mLastUpdateFrameIndex(-1)
		, mName("ParticleSystem")
		, mPlayOnAwake(true)
		, mIsPlaying(true)
		, mIsStop(false)
		, mbStepOne(false)
		, mPreDelay(0.0f)
		, mPlayRateScale(1.0f)
		, mUseExternBB(false)
		, mLiveTime(0.0)
		, mQuota( ConstDefine::DEFAULT_TECHNIQUE_QUOTA )
		, mPoolNeedIncrease(true)
		, mIsMoveWorldCoord(false)
		, mIsLoadEmitterMesh(false)
		, mspf(DefaultFrameTime)
		, mfps(30)
		, mCurrentTimeForFps(0.0f)
		, mLoop(true)
		, mDuration(10.0)
		, mDelayTime(0.0)
		, mNeedUpdate(true)
		, mUpdateUnVis(false)
		, mUpdateTarget(false)
		, mNeedUpdateBox(false)
		, mSort(0)
	{
#ifdef __GENESIS_EDITOR__	//	edtior use
		mCubeEmitter = NULL;
		mConeEmitter = NULL;
		mSphereEmitter = NULL;
		mModelEmitter = NULL;
		mBillBoardTarget = NULL;
		mRibbonTarget = NULL;
#endif

	}
	//------------------------------------------------------------------------
	ParticleSystem::~ParticleSystem()
	{

	}
	//------------------------------------------------------------------------
	void
		ParticleSystem::Active(void)
	{
		ParticleServer::Instance()->_attachParticleSystem( GPtr<ParticleSystem>(this) );
		mNeedUpdate = true;
	}
	//------------------------------------------------------------------------
	void 
		ParticleSystem::DeActive(void)
	{
		ParticleServer::Instance()->_deattachParticleSystem( GPtr<ParticleSystem>(this) );
	}
	//------------------------------------------------------------------------
	void
		ParticleSystem::_onActive()
	{
		n_assert( !mIsActive );

		n_assert( !mPool.isvalid() );
		mPool = ParticlePool::Create();
		_preparePool();

		if(mEmitter)
		{
			mEmitter->_onActivate();
		}

		for ( IndexT index = 0; index < mAffectors.Size(); ++index )
		{
			mAffectors[index]->_onActivate();
		}
		if ( mTarget )
		{
			mTarget->_onActivate();
		}

		mIsActive = true;
	}
	//------------------------------------------------------------------------
	void
		ParticleSystem::_onDeactive()
	{
		n_assert( mIsActive );
		if(mEmitter)
		{
			mEmitter->_onDeactivate();
		}
		for ( IndexT index = 0; index < mAffectors.Size(); ++index )
		{
			mAffectors[index]->_onDeactivate();
		}
		if ( mTarget )
		{
			mTarget->_onDeactivate();
		}

		mPool = NULL;
		mPoolNeedIncrease = true;

		mIsActive = false;
	}
	//------------------------------------------------------------------------
	void 
		ParticleSystem::Addtime(Timing::Time t, IndexT frameIndex)
	{
		if (!mIsPlaying)
			t = 0;
		if (mbStepOne)
		{
			t = DefaultFrameTime;
			mMobileTime += t;
			mbStepOne = false;
		}
		else
		{
			t *= mPlayRateScale;
		}
		if ( mEmitter.isvalid() )
		{
			if ( !mLoop && mLiveTime < mDuration && (mLiveTime + t) > mDuration && (mDuration - mLiveTime) > 0.002f  )
			{
				t = mDuration - mLiveTime - 1e-9;
			}
		}


		mCurFrameTime = t;
		mFrameIndex = frameIndex;
	}
	//------------------------------------------------------------------------
	void 
		ParticleSystem::Update(void)
	{
		
		if (!mNeedUpdate && !mUpdateUnVis)
		{
			mUpdateTarget = false;
			return;
		}
		
		mNeedUpdate = false;
		mUpdateTarget = true;
		Timing::Time time = App::GameTime::Instance()->GetFrameTime();
		if(mIsPlaying)
		{
			mMobileTime += time;
		}

		Timing::Time total = time;
		//====================================================================================
		//Use bbox size and camera distance to change animation's update rate
		bool bNeedLimitRate = true;
#ifdef __GENESIS_EDITOR__	//	edtior didn`t use
		bNeedLimitRate = false;
#endif
		if(bNeedLimitRate)
		{
			//_fpsControl();
			mCurrentTimeForFps += time;
			if(mCurrentTimeForFps < mspf)
			{
				return;
			}
			total = 0.0f;
			while(mCurrentTimeForFps >= mspf)
			{
				total += mspf;
				mCurrentTimeForFps -= mspf;
			}
		}
		Addtime( total,App::TimeManager::Instance()->GetFrameIndex() );

		if( mIsPlaying && mIsPreLoop && mLiveTime <= MinFrameTime)
		{
			if(mPlayBackTime > 0.0f)
			{
				SetPlayTime(mPlayBackTime);
			}
			else
			{
				float playtime = 2.0f; 
				_repeatUpdate(playtime);
				mPreDelay = playtime;
			}
		}
		if ( mLastUpdateFrameIndex != mFrameIndex )
		{
			_techUpdate(mCurFrameTime, mFrameIndex);
			mLastUpdateFrameIndex = mFrameIndex;
		}

		if(mIsPlaying && !mLoop && (GetCurEmitTime() + 0.00001 > mDuration)
			&& mPool->IsEmpty())
		{
			Stop();
		}
	}
	//--------------------------------------------------------------------------------
	bool ParticleSystem::IsDirtyPrim(IndexT nIdx) const
	{
		if(mTarget.isvalid())
			return mTarget->IsDirtyPrim();

		return false;
	}
	//--------------------------------------------------------------------------------
	void ParticleSystem::SetDirtyPrim(IndexT nIdx, bool bset)
	{
		if(mTarget.isvalid())
			mTarget->SetDirtyPrim(bset);
	}
	//--------------------------------------------------------------------------------
	void ParticleSystem::_InitTime()
	{
		//	mLiveTime = 0.f;
		mCurFrameTime = 0.f;
		mFrameIndex = 0;
		mLastUpdateFrameIndex = 0;
	}
	//--------------------------------------------------------------------------------
	void ParticleSystem::_repeatUpdate(float time)
	{
		float LiveNextFrameTime = (float)DefaultFrameTime;

		do
		{
			_techUpdate(DefaultFrameTime, mFrameIndex);
			LiveNextFrameTime += (float)DefaultFrameTime;
		} while ( LiveNextFrameTime <= time);

		_techUpdate( time - LiveNextFrameTime + DefaultFrameTime, mFrameIndex);
	}
	void ParticleSystem::_techUpdate(Timing::Time frameTime,IndexT frameIndex )
	{
		mCurFrameTime = frameTime;
		mLiveTime += mCurFrameTime;
		mFrameIndex = frameIndex;
		_preparePool();
		if(mTarget->GetTargetType() == ParticleTarget::GPU ||
			frameTime <= 0.0000001)
			return;

		_emitParticles();
		_preProcessParticles();
		_processParticles();
	}
	//------------------------------------------------------------------------
	void ParticleSystem::Stop()
	{
		mIsPlaying = false;
		mIsStop = true;
		

		Reset();
	}
	//------------------------------------------------------------------------
	void ParticleSystem::SetPlayTime(Timing::Time timePoint)
	{
		_InitTime();
		if (mIsActive)
		{
			Reset();
			_repeatUpdate((float)timePoint);
		}
	}

	//------------------------------------------------------------------------
	Math::float3
		ParticleSystem::GetDerivedPosition(void)
	{

		Math::float3 sysPos(mWorldMatrix.get_position().x(),
			mWorldMatrix.get_position().y(),
			mWorldMatrix.get_position().z());

		return sysPos;
	}
	//------------------------------------------------------------------------
	Math::float3 ParticleSystem::GetDerivedScale(void)
	{

		Math::float3 sysScale(mWorldMatrix.getrow0().x(),
			mWorldMatrix.getrow1().y(),
			mWorldMatrix.getrow2().z());

		return sysScale;
	}
	//------------------------------------------------------------------------
	Math::quaternion ParticleSystem::GetDerivedRotation(void)
	{
		Math::quaternion quat = Math::matrix44::rotationmatrix(mWorldMatrix);
		return quat;
	}
	//------------------------------------------------------------------------ 
	void
		ParticleSystem::SetParticleQuota(SizeT quota)
	{
		if ( quota <= 0 )
		{
			n_warning(" ParticleTechnique::SetParticleQuota, should bigger than 0");
			quota = 1; 
		}
		if ( quota > ConstDefine::MAX_TECHNIQUE_QUOTA )
		{
			n_warning(" ParticleTechnique::SetParticleQuota, should smaller than %d", ConstDefine::MAX_TECHNIQUE_QUOTA );
			quota = ConstDefine::MAX_TECHNIQUE_QUOTA;
		}
		mQuota = quota;
		mPoolNeedIncrease = true;
	}

	//------------------------------------------------------------------------
	void ParticleSystem::Reset()
	{
		mLiveTime = 0;
		mFrameIndex = 0;
		mCurFrameTime = 0;
		mLastUpdateFrameIndex = -1;
		ParticleSystemDataChanged();
		_resetPool(mQuota);
		mEmitter->Reset();
	}

	//------------------------------------------------------------------------
	void 
		ParticleSystem::_emitParticles(void)
	{
		if( (!mIsPreLoop && mLiveTime < mDelayTime) || !mEmitter )
			return;
		if(!mLoop && (mDuration + mDelayTime) < mLiveTime)
			return ;

		SizeT requested = mEmitter->CalculateRequestedParticles(mCurFrameTime);
		_executeEmitParticles( mEmitter, requested );
	}

	//------------------------------------------------------------------------
	void 
		ParticleSystem::_executeEmitParticles(ParticleEmitterPtr& emitter, SizeT requested)
	{
		n_assert( mPool.isvalid() );
		n_assert( emitter.isvalid() );

		if ( requested == 0 )
		{
			return;
		}

		for ( IndexT index = 0; index < requested; ++index )
		{
			Particle* particle = mPool->ReleaseParticle();

			if ( !particle )
			{
				return;
			}

			emitter->_emit( particle,(float)GetCurEmitTime());
			_initParticleForEmission(particle);
		}
	}

	//------------------------------------------------------------------------
	void ParticleSystem::_preProcessParticles(void)
	{
		for ( IndexT index = 0; index < mAffectors.Size(); ++index)
		{
			mAffectors[index]->_preProcessParticles();
		}
	}
	//------------------------------------------------------------------------
	void ParticleSystem::_processParticles(void)
	{
		if ( mPool->IsEmpty())
			return;

		Particle* particle			= mPool->GetFirst();
		ParticleEmitter* emitter	= 0;
		bool firstParticle			= true; 
		bool firstActiveParticle	= true;

		while ( !mPool->End() )
		{
			if (particle)
			{
				if ( !_isExpired(particle, mCurFrameTime) )
				{
					_processAffectors(particle, firstActiveParticle);

					firstActiveParticle = false;

				}
				else
				{
					//add by zhangjitao
					particle->mOrbitPositions.Reset();
					mPool->LockLatestParticle();
				}

				// Decrement time to live
				particle->mTimeToLive -= (Math::scalar)mCurFrameTime;

				particle->mTimeFraction = (particle->mTotalTimeToLive - particle->mTimeToLive)/particle->mTotalTimeToLive;
			}

			firstParticle = false;
			particle = mPool->GetNext();
		}
	}
	//-----------------------------------------------------------------------
	void ParticleSystem::_initParticleForEmission(Particle* particle)
	{
		if ( !mAffectors.IsEmpty() )
		{
			for ( IndexT index = 0; index < mAffectors.Size(); ++index)
			{
				mAffectors[index]->_initParticleForEmission(particle);
			}
		}
	}
	//------------------------------------------------------------------------
	void ParticleSystem::_processAffectors(Particle* particle, bool firstActiveParticle)
	{
		if ( !mAffectors.IsEmpty() )
		{
			for ( IndexT index = 0; index < mAffectors.Size(); ++index)
			{
				mAffectors[index]->_processParticle(particle, firstActiveParticle);
			}
		}
	}
	//------------------------------------------------------------------------
	void ParticleSystem::_postProcessParticles(void)
	{
		if ( (!mUpdateTarget || mPoolNeedIncrease) && 
			mTarget->GetTargetType() != ParticleTarget::GPU )
		{
			return;
		}
		for ( IndexT index = 0; index < mAffectors.Size(); ++index)
		{
			mAffectors[index]->_postProcessParticles();
		}
		if ( mTarget.isvalid() && mPool.isvalid())
		{
			mTarget->_updateTarget( mPool, mCurFrameTime );
		}
	}
	//------------------------------------------------------------------------
	void ParticleSystem::_switchToMoveWorldCoord(bool isWorld)
	{
		if(mIsMoveWorldCoord == isWorld || NULL == mPool)
			return;
		Particle* particle = mPool->GetFirst();

		Math::matrix44 techMat = mWorldMatrix;
		if(!isWorld)
			techMat = Math::matrix44::inverse(techMat);
		while ( particle )
		{
			particle->mPosition = particle->mPosition.transformPoint(techMat);
			particle = mPool->GetNext();
		}
	}
	//------------------------------------------------------------------------
	const ParticleEmitterPtr& 
		ParticleSystem::GetEmitter() const
	{
		return mEmitter;
	}
	//------------------------------------------------------------------------
	const ParticleAffectorPtr& 
		ParticleSystem::GetAffector(IndexT index) const
	{
		if ( index >=0 && index < mAffectors.Size() )
		{
			return mAffectors[index];
		}
		else
		{
			return ParticleAffector::NullAffector;
		}
	}

	//------------------------------------------------------------------------
	void 
		ParticleSystem::AddAffector( const ParticleAffectorPtr& affector, IndexT index)
	{
		if ( !affector.isvalid() )
		{
			return;
		}

		if ( affector->GetParentSystem() != NULL )
		{
			return;
		}

		if (index >= 0)
			mAffectors.Insert( index, affector );
		else
			mAffectors.Append( affector );

		affector->SetParentSystem( this );

		if ( IsActive() )
		{
			affector->_onActivate();
		}
	}
	//------------------------------------------------------------------------
	void
		ParticleSystem::RemoveAffector( IndexT index )
	{
		if ( index >= 0 && index < mAffectors.Size() )
		{
			ParticleAffectorPtr& affector = mAffectors[index];
			n_assert(affector.isvalid());
			if ( affector->IsActive() )
			{
				affector->_onDeactivate();
			}
			affector->SetParentSystem(NULL);
			mAffectors.EraseIndex( index );
		}
	}
	//------------------------------------------------------------------------
	void 
		ParticleSystem::RemoveAllAffector(void)
	{
		for ( IndexT index = 0; index < mAffectors.Size(); ++index)
		{
			ParticleAffectorPtr& affector = mAffectors[index];
			n_assert(affector.isvalid());
			if ( affector->IsActive() )
			{
				affector->_onDeactivate();
			}
			affector->SetParentSystem(NULL);
		}
		mAffectors.Clear();
	}

	//------------------------------------------------------------------------
	void 
		ParticleSystem::SetTarget( const ParticleTargetPtr& target)
	{
		if(mTarget.isvalid())
			RemoveTarget();
		if ( target.isvalid() )
		{
			if ( target->GetParentSystem() != NULL )
			{
				return;
			}
		}

		if ( mTarget.isvalid() )
		{
			if ( mTarget->IsActive() )
			{
				mTarget->_onDeactivate();
			}
			mTarget->SetParentSystem(NULL);
			mTarget = NULL;
		}

		n_assert( !mTarget.isvalid() );
		mTarget = target;

		if ( mTarget.isvalid() )
		{
			mTarget->SetParentSystem(this);
			if ( IsActive() )
			{
				mTarget->_onActivate();
			}
		}

#ifdef __GENESIS_EDITOR__	//	edtior use
		uint emtfourcc = mTarget->GetClassFourCC().AsUInt();	
		switch(emtfourcc)
		{
		case CPFCC::TARGET_BILLBOARD:
			if(mBillBoardTarget.isvalid())
			{
				GPtr<ParticleBillBoardTarget> billboardTarget = mTarget.downcast<ParticleBillBoardTarget>();
				billboardTarget->SetBillBoardAlignType(mBillBoardTarget->GetBillBoardAlignType());
				billboardTarget->SetBillBoardType(mBillBoardTarget->GetBillBoardType());
				billboardTarget->SetStretchScale(mBillBoardTarget->GetStretchScale());
				billboardTarget->SetOrientType(mBillBoardTarget->GetOrientType());
			}
			break;
		case CPFCC::TARGET_RIBBONTRAIL:
			if(mRibbonTarget.isvalid())
			{
				GPtr<RibbonTrailTarget> ribbonTarget = mTarget.downcast<RibbonTrailTarget>();
				ribbonTarget->SetTrailLength(mRibbonTarget->GetTrailLength());
				ribbonTarget->SetMaxElements(mRibbonTarget->GetMaxElements());
			}
			break;
		}	
#endif

	}
	//--------------------------------------------------------------------------------
	void ParticleSystem::RemoveTarget()
	{

		if( !mTarget.isvalid())
			return;
#ifdef __GENESIS_EDITOR__	//	edtior use
		uint emtfourcc = mTarget->GetClassFourCC().AsUInt();	
		switch(emtfourcc)
		{
		case CPFCC::TARGET_BILLBOARD:
			mBillBoardTarget = mTarget.downcast<ParticleBillBoardTarget>();
			break;
		case CPFCC::TARGET_RIBBONTRAIL:
			mRibbonTarget = mTarget.downcast<RibbonTrailTarget>();
			break;
		}	
#endif
		if ( mTarget->IsActive() )
		{
			mTarget->_onDeactivate();
		}
		mTarget->SetParentSystem(NULL);
		mTarget = NULL;
	}

	//------------------------------------------------------------------------
	void 
		ParticleSystem::RemoveEmitter()
	{
		if ( mEmitter )
		{
#ifdef __GENESIS_EDITOR__	//	edtior use
			uint emtfourcc = mEmitter->GetClassFourCC().AsUInt();	
			switch(emtfourcc)
			{
			case CPFCC::EMITTER_BOX:
				mCubeEmitter = mEmitter.downcast<BoxEmitter>();
				break;
			case CPFCC::EMITTER_SPHERESURFACE:
				mSphereEmitter = mEmitter.downcast<SphereSurfaceEmitter>();
				break;
			case CPFCC::EMITTER_CONE:
				mConeEmitter = mEmitter.downcast<ConeEmitter>();
				break;
			case CPFCC::EMITTER_MODEL:
				mModelEmitter = mEmitter.downcast<ModelEmitter>();
				break;
			}	
#endif
			n_assert(mEmitter.isvalid());
			if ( mEmitter->IsActive() )
			{
				mEmitter->_onDeactivate();
			}
			mEmitter->SetParentSystem(NULL);
			mEmitter = NULL;
		}

	}

	//------------------------------------------------------------------------
	void 
		ParticleSystem::SetEmitter( const ParticleEmitterPtr& emitter)
	{
		if ( !emitter.isvalid() )
		{
			return;
		}

		if ( emitter->GetParentSystem() != NULL )
		{
			return;
		}
		mEmitter = emitter;
		emitter->SetParentSystem( this );

		if ( IsActive() )
		{
			emitter->_onActivate();
		}
#ifdef __GENESIS_EDITOR__	//	edtior use
		uint emtfourcc = emitter->GetClassFourCC().AsUInt();	
		switch(emtfourcc)
		{
		case CPFCC::EMITTER_BOX:
			if(mCubeEmitter.isvalid())
			{
				mEmitter->SetNormalDir(mCubeEmitter->IsNormalDir());
				mEmitter->SetNormalSpeed(mCubeEmitter->GetNormalSpeed());
				mEmitter->SetParticleInitFromType(mCubeEmitter->GetParticleInitFromType());
				emitter->getMinMaxCurve(Emitter_BoxSizeX)->CopyFrom(*mCubeEmitter->getMinMaxCurve(Emitter_BoxSizeX));
				emitter->getMinMaxCurve(Emitter_BoxSizeY)->CopyFrom(*mCubeEmitter->getMinMaxCurve(Emitter_BoxSizeY));
				emitter->getMinMaxCurve(Emitter_BoxSizeZ)->CopyFrom(*mCubeEmitter->getMinMaxCurve(Emitter_BoxSizeZ));

			}
			break;
		case CPFCC::EMITTER_SPHERESURFACE:
			if(mSphereEmitter.isvalid())
			{
				mEmitter->SetNormalDir(mSphereEmitter->IsNormalDir());
				mEmitter->SetNormalSpeed(mSphereEmitter->GetNormalSpeed());
				mEmitter->SetParticleInitFromType(mSphereEmitter->GetParticleInitFromType());
				emitter->getMinMaxCurve(Emitter_SphereRadius)->CopyFrom(*mSphereEmitter->getMinMaxCurve(Emitter_SphereRadius));
				emitter->getMinMaxCurve(Emitter_SphereHem)->CopyFrom(*mSphereEmitter->getMinMaxCurve(Emitter_SphereHem));
				emitter->getMinMaxCurve(Emitter_SphereSlice)->CopyFrom(*mSphereEmitter->getMinMaxCurve(Emitter_SphereSlice));

			}
			break;
		case CPFCC::EMITTER_CONE:
			if(mConeEmitter.isvalid())
			{
				mEmitter->SetNormalDir(mConeEmitter->IsNormalDir());
				mEmitter->SetNormalSpeed(mConeEmitter->GetNormalSpeed());
				mEmitter->SetParticleInitFromType(mConeEmitter->GetParticleInitFromType());
				emitter->getMinMaxCurve(Emitter_ConeRadiusInner)->CopyFrom(*mConeEmitter->getMinMaxCurve(Emitter_ConeRadiusInner));
				emitter->getMinMaxCurve(Emitter_ConeRadiusOuter)->CopyFrom(*mConeEmitter->getMinMaxCurve(Emitter_ConeRadiusOuter));
				emitter->getMinMaxCurve(Emitter_ConeHeight)->CopyFrom(*mConeEmitter->getMinMaxCurve(Emitter_ConeHeight));
				emitter->getMinMaxCurve(Emitter_ConeAngle)->CopyFrom(*mConeEmitter->getMinMaxCurve(Emitter_ConeAngle));
			}
			break;
		case CPFCC::EMITTER_MODEL:
			if(mModelEmitter.isvalid())
			{
				mEmitter->SetNormalDir(mModelEmitter->IsNormalDir());
				mEmitter->SetNormalSpeed(mModelEmitter->GetNormalSpeed());
				mEmitter->SetParticleInitFromType(mModelEmitter->GetParticleInitFromType());

				GPtr<ModelEmitter> modelEmitter = mEmitter.downcast<ModelEmitter>();
				modelEmitter->SetMeshName(mModelEmitter->GetMeshName());
			}
			break;
		}	
#endif


	}
	//------------------------------------------------------------------------
	void ParticleSystem::_preparePool(void)
	{
		if ( mPoolNeedIncrease)
		{
			n_assert( mPool.isvalid() );
			mPool->DestroyAllVisualParticles();
			mPool->IncreaseVisualParticlePool( mQuota );
			mPoolNeedIncrease = false;
		}
	}
	//--------------------------------------------------------------------------------
	void ParticleSystem::_resetPool(int nQuota)
	{
		mPool->DestroyAllVisualParticles();
		SetParticleQuota(nQuota);	
	}
	//-------------------------------------------------------------------------
	Timing::Time ParticleSystem::GetCurEmitTime()
	{
		double curEmitTime = 0;
		Timing::Time delayTime = mDelayTime;

		bool isLoop = mLoop;

		if (mDuration > 0)
		{
			double tmp = mLiveTime - delayTime;
			if (tmp <= 0)
			{
				curEmitTime = 0;
			}
			else
			{
				if (tmp <= mDuration)
				{
					curEmitTime = tmp;
				}
				else
				{
					if (isLoop)
					{
						int tmpInt = (int)(tmp / mDuration);
						curEmitTime = tmp - tmpInt * mDuration;
					}
					else
					{
						curEmitTime = mDuration;
					}
				}
			}
		}
		else
		{
			curEmitTime = 0;
		}

		return curEmitTime;
	}
	Timing::Time ParticleSystem::GetCorrectTime(void) const
	{
		Timing::Time delayTime = mDelayTime;

		if (mIsPreLoop)
		{
			delayTime = 0.0f;
		}
		return delayTime;
	}
	void ParticleSystem::SetupSpriteGPUParticles(SpriteGPUParticleVertex* particleVertex,int quato)
	{
		float rate = mEmitter->GetGPUParticleRate();
		float life = mEmitter->GetGPUParticleLife();
		mDuration = life;
		float rateFps = 1.0f/rate;
		int totalPar = 0;
		Particle* particle = new Particle();
		float _time = (float)App::GameTime::Instance()->GetTime() + (float)mDelayTime;
		if(mIsPreLoop && mIsPlaying)
			_time -= life;
		Math::Color32 _color;
		Math::bbox _box;
		_box.begin_extend();
		while (totalPar < quato)
		{
			mEmitter->_emit(particle,(float)mDuration*totalPar/quato);
			particleVertex[totalPar].mColor = particle->mColor.HexARGB();
			particleVertex[totalPar].mPosition = particle->mPosition;
			particleVertex[totalPar].mSize = particle->mSize.x();
			particleVertex[totalPar].mNormal = particle->mDirection;
			particleVertex[totalPar].mTexCoord.set(_time,particle->mTotalTimeToLive);
			_time += rateFps;
			totalPar ++;

			_box.extend(Math::point(
				particle->mPosition.x(),particle->mPosition.y(),particle->mPosition.z()));
			Math::float3 endPos;
			endPos = _getEndPos(particle->mPosition,particle->mDirection,life);
			_box.extend(Math::point(
				endPos.x(),endPos.y(),endPos.z()));
		}
		_box.end_extend();
		_box.pmax += Math::point(
			particle->mSize.x()*0.5f,particle->mSize.y()*0.5f,particle->mSize.z()*0.5f);
		_box.pmin -= Math::point(
			particle->mSize.x()*0.5f,particle->mSize.y()*0.5f,particle->mSize.z()*0.5f);
		SetLocalBoundingBox(_box);
		delete particle;
	}
	void ParticleSystem::SetupBoardGPUParticles(BoardGPUParticleVertex* particleVertex,ushort* indicies,int quato)
	{
		float rate = mEmitter->GetGPUParticleRate();
		float life = mEmitter->GetGPUParticleLife();
		mDuration = life;
		float rateFps = 1.0f/rate;
		int totalPar = 0;
		Particle* particle = new Particle();
		float _time = (float)mDelayTime;
		if(mIsPreLoop && mIsPlaying)
			_time = -life;
		mMobileTime = 0;
		Math::Color32 _color;
		Math::bbox _box;
		float halfXSize = 0;
		float halfYSize = 0;
		_box.begin_extend();
		while (totalPar < quato)
		{
			mEmitter->_emit(particle,(float)mDuration*totalPar/quato);
			halfXSize = 0.5f*particle->mSize.x();
			halfYSize = 0.5f*particle->mSize.y();
			particleVertex[4*totalPar].mColor = particle->mColor.HexARGB();
			particleVertex[4*totalPar].mPosition = particle->mPosition;
			particleVertex[4*totalPar].mTangent.set(-halfXSize,halfYSize,0,0);
			particleVertex[4*totalPar].mNormal = particle->mDirection;
			particleVertex[4*totalPar].mTexCoord.set(_time,particle->mTotalTimeToLive);

			particleVertex[4*totalPar+1] = particleVertex[4*totalPar];
			particleVertex[4*totalPar+1].mTangent.set(-halfXSize,-halfYSize,0,1);

			particleVertex[4*totalPar+2] = particleVertex[4*totalPar];
			particleVertex[4*totalPar+2].mTangent.set(halfXSize,-halfYSize,1,1);

			particleVertex[4*totalPar+3] = particleVertex[4*totalPar];
			particleVertex[4*totalPar+3].mTangent.set(halfXSize,halfYSize,1,0);

			indicies[6*totalPar]   = 4*totalPar;
			indicies[6*totalPar+1] = 4*totalPar + 1;
			indicies[6*totalPar+2] = 4*totalPar + 2;
			indicies[6*totalPar+3] = 4*totalPar + 2;
			indicies[6*totalPar+4] = 4*totalPar + 3;
			indicies[6*totalPar+5] = 4*totalPar ;

			_time += rateFps;
			totalPar ++;

			_box.extend(Math::point(
				particle->mPosition.x(),particle->mPosition.y(),particle->mPosition.z()));
			Math::float3 endPos;
			endPos = _getEndPos(particle->mPosition,particle->mDirection,life);
			_box.extend(Math::point(
				endPos.x(),endPos.y(),endPos.z()));
		}
		_box.end_extend();
		_box.pmax += Math::point(
			particle->mSize.x()*0.5f,particle->mSize.y()*0.5f,particle->mSize.z()*0.5f);
		_box.pmin -= Math::point(
			particle->mSize.x()*0.5f,particle->mSize.y()*0.5f,particle->mSize.z()*0.5f);
		SetLocalBoundingBox(_box);
		delete particle;
	}
	void ParticleSystem::ParticleSystemDataChanged()
	{
		if(mTarget->GetTargetType() == ParticleTarget::GPU)
			mTarget->SetNeedPrimitive(true);
	}
	void ParticleSystem::SetShaderParam()
	{
		Graphic::GlobalMaterialParam* pGMP = Graphic::Material::GetGlobalMaterialParams();
		pGMP->SetVectorParam(Graphic::eGShaderParticleTime,Math::float4(0,0,0,(float)mMobileTime));
		for ( IndexT index = 0; index < mAffectors.Size(); ++index)
		{
			mAffectors[index]->SetShaderParam();
		}
	}
	//------------------------------------------------------------------
	void ParticleSystem::_fpsControl()
	{
		Math::bbox box = GetBoundingBox();
		Math::float4 boxSizeVec = box.pmax - box.pmin;
		float boxSize = boxSizeVec.x()*boxSizeVec.x()+boxSizeVec.y()*boxSizeVec.y()+boxSizeVec.z()*boxSizeVec.z();
		
		Graphic::Camera* pMainCam = Graphic::GraphicSystem::Instance()->GetSceneDefaultCamera();
		if(pMainCam == NULL)
			return;
		Math::vector cameraLocalPos = pMainCam->GetTransform().get_position() - mWorldMatrix.get_position();
		float dis = cameraLocalPos.x()*cameraLocalPos.x()+cameraLocalPos.y()*cameraLocalPos.y()+cameraLocalPos.z()*cameraLocalPos.z();

		float zNear = pMainCam->GetCameraSetting().GetZNear();
		float zFar = pMainCam->GetCameraSetting().GetZFar();
		float farWidth = pMainCam->GetCameraSetting().GetFarWidth();
		float farHeight = pMainCam->GetCameraSetting().GetFarHeight();
		float viewSize = dis/((zFar-zNear)*(zFar-zNear))*(farWidth*farWidth + farHeight*farHeight);

		float updateTime = MinUpdateTime + (MaxUpdateTime-MinUpdateTime)* 0.001f * viewSize/boxSize ;
		updateTime = updateTime>MaxUpdateTime?MaxUpdateTime:updateTime;

		mspf = updateTime*UpdateFactor;
		mfps = (int)(1.0f/mspf);
	}
	//--------------------------------------------------------------------
	void ParticleSystem::InitShaderParam()
	{
		for ( IndexT index = 0; index < mAffectors.Size(); ++index)
		{
			mAffectors[index]->InitShaderParam();
		}
	}
	//------------------------------------------------------------------
	void ParticleSystem::_switchParticleType(bool _isMobile)
	{
		if(_isMobile)
		{
			GPtr<ParticleGPUTarget> _target = ParticleGPUTarget::Create();
			SetTarget(_target.downcast<ParticleTarget>());
		}
		else
		{
			GPtr<ParticleBillBoardTarget> _target = ParticleBillBoardTarget::Create();
			SetTarget(_target.downcast<ParticleTarget>());
		}
		mEmitter->_switchParticleType(_isMobile);
		for ( IndexT index = 0; index < mAffectors.Size(); ++index)
		{
			mAffectors[index]->_switchParticleType(_isMobile);
		}
	}
	//--------------------------------------------------------------------------------
	void ParticleSystem::SetShaderMask(const GPtr<ShaderProgramCompiler::ShaderMarcro>& pMarcro)
	{
		for ( IndexT index = 0; index < mAffectors.Size(); ++index)
		{
			mAffectors[index]->SetShaderMask(pMarcro);
		}
	}
	//--------------------------------------------------------------------------------
	const Math::float3 ParticleSystem::_getEndPos(const Math::float3& pos,const Math::float3 speed,float time)
	{
		ParticleAffectorPtr affector = NULL;
		Math::float3 endPos = pos + speed*time;
		for ( IndexT index = 0; index < mAffectors.Size(); ++index)
		{
			affector = mAffectors[index];
			endPos = affector->_getEndPos(pos,speed,time);
		}
		return endPos;
	}
}

