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
#include "particleLinearForceAffector.h"
#include "particles/particlesystem.h"
#include "particles/particleaffector.h"
#include "particles/particle.h"

namespace Particles
{
	using namespace Math;

	__ImplementClass(Particles::LinearForceAffector, CPFCC::AFFECTOR_LINEARFORCE, Particles::ParticleAffector);

	//-----------------------------------------------------------------------
	LinearForceAffector::LinearForceAffector(void) : ParticleAffector()
		, mForceApplication(LinearForceAffector::FA_ADD)
		, mSpaceCoord(SCT_LOCAL)
	{
		mName = "LinearForceAffector";
		mAffectType = AT_Force;
	}
	//-----------------------------------------------------------------------
	void LinearForceAffector::_preProcessParticles()
	{
	}
	//-----------------------------------------------------------------------
	void LinearForceAffector::_affect(Particle* particle)
	{		
		if(!GetEnable())
			return;
		mTimeSinceLastUpdate += (Math::scalar)mParentSystem->GetCurrentFrameTime();
		if (mTimeSinceLastUpdate >= mTimeStep)
		{

			float percent = particle->mTimeFraction;

			Math::float3 forceVec(mForceVectorX.Calculate(percent,particle->mRandom0) ,
				mForceVectorY.Calculate(percent,particle->mRandom1),
				mForceVectorZ.Calculate(percent,particle->mRandom2) );

			if  ( mSpaceCoord == SCT_WORLD  )
			{
				forceVec = forceVec.transformVector(Math::matrix44::inverse(mParentSystem->GetWorldMatrix()));
			}


			if (mForceApplication == FA_ADD)
			{
				particle->mDirection += forceVec * (Math::scalar)mParentSystem->GetCurrentFrameTime();
			}
			else
			{
				particle->mDirection = (particle->mDirection + forceVec * (Math::scalar)mParentSystem->GetCurrentFrameTime()) / 2;
			}


			mTimeSinceLastUpdate = 0;
		}

	}
	//--------------------------------------------------------------------------------
	void LinearForceAffector::InitShaderParam()
	{
		mShaderParam.x() = mForceVectorX.Calculate(0,Math::n_rand(0,1));
		mShaderParam.y() = mForceVectorY.Calculate(0,Math::n_rand(0,1));
		mShaderParam.z() = mForceVectorZ.Calculate(0,Math::n_rand(0,1));
	}
	void LinearForceAffector::SetShaderParam()
	{
		Graphic::GlobalMaterialParam* pGMP = Graphic::Material::GetGlobalMaterialParams();
		Math::float4 _shaderPar = pGMP->GetVectorParam(Graphic::eGShaderParticleForce);
		_shaderPar.x() = mShaderParam.x();
		_shaderPar.y() = mShaderParam.y();
		_shaderPar.z() = mShaderParam.z();
		pGMP->SetVectorParam(Graphic::eGShaderParticleForce,_shaderPar);
	}
	//--------------------------------------------------------------------------------
	void LinearForceAffector::SetShaderMask(const GPtr<ShaderProgramCompiler::ShaderMarcro>& pMarcro)
	{
		if(!GetEnable())
			return;
		pMarcro->TurnOn(ShaderProgramCompiler::ShaderMarcro::m_sParticleForce);
	}
	//--------------------------------------------------------------------------------
	const Math::float3 LinearForceAffector::_getEndPos(const Math::float3& pos,const Math::float3 speed,float time)
	{
		Math::float3 endPos;
		endPos = pos + speed*time + 0.5*mShaderParam*time*time;
		return endPos;
	}
	//--------------------------------------------------------------------------------
	Math::MinMaxCurve* LinearForceAffector::getMinMaxCurve(ParticleCurveType pct)
	{
		switch(pct)
		{
		case 	Affector_ForceX:
			return &mForceVectorX;
		case 	Affector_ForceY:
			return &mForceVectorY;
		case Affector_ForceZ:
			return &mForceVectorZ;

		default:
			return Super::getMinMaxCurve(pct);					
		}
	}
	//-------------------------------------------------------------------------------
	void LinearForceAffector::SwitchParticleType(bool _isMobile)
	{
		if(!_isMobile)
		{
			mSpaceCoord = SCT_LOCAL;
		}
	}
}
