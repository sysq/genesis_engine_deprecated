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
#include "particleScaleAffector.h"
#include "particles/particleaffector.h"
#include "particles/particlesystem.h"
#include "particles/particle.h"

namespace Particles
{
	using namespace Math;

	__ImplementClass(Particles::ScaleAffector, CPFCC::AFFECTOR_SCALE, Particles::ParticleAffector);

	//-----------------------------------------------------------------------
	ScaleAffector::ScaleAffector(void) : ParticleAffector()
		, mIsAxialScale(false)
	{
		mAffectType = AT_Scale;
		mScaleX.SetScalar(1.0f);
		mScaleY.SetScalar(1.0f);
		mScaleZ.SetScalar(1.0f);

		mName = "ScaleAffector";
	}

	//-----------------------------------------------------------------------
	void ScaleAffector::_affect(Particle* particle)
	{
		if( !GetEnable())
			return;
		float percent = particle->mTimeFraction;

		Math::float3 particleSize(mScaleX.Calculate(percent,particle->mRandom0),
			mScaleY.Calculate(percent,particle->mRandom1),
			mScaleZ.Calculate(percent,particle->mRandom2) );


		if(!mIsAxialScale)
		{
			particleSize.y() = particleSize.x();
			particleSize.z() = particleSize.x();
		}
		particle->mSize = particle->mInitSize * particleSize;

	}
	//-------------------------------------------------------------------------------
	void ScaleAffector::SetShaderParam()
	{
		Graphic::GlobalMaterialParam* pGMP = Graphic::Material::GetGlobalMaterialParams();
		Math::float4 _shaderPar = pGMP->GetVectorParam(Graphic::eGShaderParticleForce);
		_shaderPar.w() = mShaderParam;
		pGMP->SetVectorParam(Graphic::eGShaderParticleForce,_shaderPar);
	}
	void ScaleAffector::InitShaderParam()
	{
		mShaderParam = mScaleX.Calculate(0,Math::n_rand(0,1));
	}
	//--------------------------------------------------------------------------------
	void ScaleAffector::SetShaderMask(const GPtr<ShaderProgramCompiler::ShaderMarcro>& pMarcro)
	{
		if(!GetEnable())
			return;
		pMarcro->TurnOn(ShaderProgramCompiler::ShaderMarcro::m_sParticleScale);
	}
	//--------------------------------------------------------------------------------
	Math::MinMaxCurve* ScaleAffector::getMinMaxCurve(ParticleCurveType pct)
	{
		switch(pct)
		{
		case 	Affector_ScaleX:
			return &mScaleX;
		case	Affector_ScaleY:
			return &mScaleY;
		case	Affector_ScaleZ:
			return &mScaleZ;

		default:
			return Super::getMinMaxCurve(pct);					
		}
	}
	//---------------------------------------------------------------------------------
	void ScaleAffector::_switchParticleType(bool _isMobile)
	{
		if(!_isMobile)
		{
			Util::Array<Math::float2> _list;
			Util::Array<bool> _type;
			_type.Append(true);
			_type.Append(true);
			Math::MinMaxCurve* curve = mParentSystem->GetEmitter()->getMinMaxCurve(Emitter_SizeX);
			_list.Append(Math::float2(0,curve->Calculate(0)));
			_list.Append(Math::float2(1,mScaleX.Calculate(0)));
			mScaleX.SetCurveFromArray(_list,_type,_list,_type);
			mScaleY.SetCurveFromArray(_list,_type,_list,_type);
			mScaleZ.SetCurveFromArray(_list,_type,_list,_type);
			return;
		}
		Math::MinMaxCurve* curve = mParentSystem->GetEmitter()->getMinMaxCurve(Emitter_SizeX);
		curve->SetCurveState(MinMaxCurve::Scalar);
		curve->SetScalar(mScaleX.Calculate(0));
		float size = mScaleX.Calculate(1);
		mScaleX.SetCurveState(MinMaxCurve::Scalar);
		mScaleX.SetScalar(size);
	}
}

