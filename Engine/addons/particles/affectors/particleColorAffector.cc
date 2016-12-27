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
#include "particleColorAffector.h"
#include "particles/particleaffector.h"
#include "particles/particle.h"
#include "graphicsystem/Material/Material.h"
#include "particles/particleemitter.h"
#include "particles/particlesystem.h"

namespace Particles
{
	using namespace Math;

	__ImplementClass(Particles::ColorAffector, CPFCC::AFFECTOR_COLOR, Particles::ParticleAffector);

	const ColorAffector::ColorOperation ColorAffector::DEFAULT_COLOR_OPERATION = ColorAffector::CAO_SET;

	//-----------------------------------------------------------------------
	ColorAffector::ColorAffector(void) : ParticleAffector(),mMinAlpha(0.0f),mMaxAlpha(1.0f),mTimeAlpha(false)
		, mColorCtlType(CCT_GRADCURVE)
	{
		mAffectType = AT_Color;
		mMinMaxColorA.SetCurveState(Math::MinMaxCurve::Scalar);	
		mMinMaxColorR.SetCurveState(Math::MinMaxCurve::Scalar);
		mMinMaxColorG.SetCurveState(Math::MinMaxCurve::Scalar);
		mMinMaxColorB.SetCurveState(Math::MinMaxCurve::Scalar);


		mName = "ColorAffector";

		Util::Array<Math::float2> _list;
		Util::Array<bool> _type;
		_list.Append(Math::float2(0,1));
		_type.Append(true);
		_type.Append(true);
		mMinMaxColorA.SetCurveFromArray(_list,_type,_list,_type);
		_list.Clear();
		_list.Append(Math::float2(0,0.5));
		mMinMaxColorB.SetCurveFromArray(_list,_type,_list,_type);
		mMinMaxColorG.SetCurveFromArray(_list,_type,_list,_type);
		mMinMaxColorR.SetCurveFromArray(_list,_type,_list,_type);

		mMinMaxColorR.SetRandomScalar(Math::float2(0.5f,0.5f));
		mMinMaxColorG.SetRandomScalar(Math::float2(0.5f,0.5f));
		mMinMaxColorB.SetRandomScalar(Math::float2(0.5f,0.5f));
		mMinMaxColorA.SetRandomScalar(Math::float2(1.0f,1.0f));
	}
	//-----------------------------------------------------------------------
	void ColorAffector::_affect(Particle* particle)
	{		
		if(!GetEnable())
			return;
		Math::Color32 clr;

		float percent = particle->mTimeFraction;

		float randomSid = Math::n_rand(0.0f,1.0f);
		clr.r =  Math::n_scalartoByte(mMinMaxColorR.Calculate(percent,randomSid));
		clr.g =  Math::n_scalartoByte(mMinMaxColorG.Calculate(percent,randomSid));
		clr.b =  Math::n_scalartoByte(mMinMaxColorB.Calculate(percent,randomSid)) ;
		clr.a =  Math::n_scalartoByte(mMinMaxColorA.Calculate(percent,randomSid));

		particle->mColor = clr;

	}
	//------------------------------------------------------------------------
	void ColorAffector::_preProcessParticles(void)
	{
		
	}
	//-------------------------------------------------------------------------------
	void ColorAffector::SetShaderParam()
	{
		Graphic::GlobalMaterialParam* pGMP = Graphic::Material::GetGlobalMaterialParams();
		pGMP->SetVectorParam(Graphic::eGShaderParticleColor,mShaderParam);
	}
	void ColorAffector::InitShaderParam()
	{
		float randomSid = Math::n_rand(0.0f,1.0f);
		mShaderParam.x() = mMinMaxColorR.Calculate(0,randomSid);
		mShaderParam.y() = mMinMaxColorG.Calculate(0,randomSid);
		mShaderParam.z() = mMinMaxColorB.Calculate(0,randomSid);
		mShaderParam.w() = mMinMaxColorA.Calculate(0,randomSid);			
	}
	//--------------------------------------------------------------------------------
	void ColorAffector::SetShaderMask(const GPtr<ShaderProgramCompiler::ShaderMarcro>& pMarcro)
	{
		if(!GetEnable())
			return;
		pMarcro->TurnOn(ShaderProgramCompiler::ShaderMarcro::m_sParticleColor);
	}
	//--------------------------------------------------------------------------------
	Math::MinMaxCurve* ColorAffector::getMinMaxCurve(ParticleCurveType pct)
	{
		switch(pct)
		{
		case	Affector_ColorR:
			return &mMinMaxColorR;
		case	Affector_ColorG:
			return &mMinMaxColorG;
		case	Affector_ColorB:
			return &mMinMaxColorB;
		case	Affector_ColorA:
			return &mMinMaxColorA;

		default:
			return Super::getMinMaxCurve(pct);					
		}
	}
	//---------------------------------------------------------------------------------
	void ColorAffector::SwitchParticleType(bool _isMobile)
	{
		if(_isMobile)
		{
			mShaderParam.x() = mMinMaxColorR.Calculate(1);
			mShaderParam.y() = mMinMaxColorG.Calculate(1);
			mShaderParam.z() = mMinMaxColorB.Calculate(1);
			mShaderParam.w() = mMinMaxColorA.Calculate(1);
			Math::MinMaxCurve* curve = mParentSystem->GetEmitter()->getMinMaxCurve(Emitter_ColorA);
			curve->SetCurveState(MinMaxCurve::Scalar);
			curve->SetScalar(mMinMaxColorA.Calculate(0));

			curve = mParentSystem->GetEmitter()->getMinMaxCurve(Emitter_ColorB);
			curve->SetCurveState(MinMaxCurve::Scalar);
			curve->SetScalar(mMinMaxColorB.Calculate(0));

			curve = mParentSystem->GetEmitter()->getMinMaxCurve(Emitter_ColorG);
			curve->SetCurveState(MinMaxCurve::Scalar);
			curve->SetScalar(mMinMaxColorG.Calculate(0));

			curve = mParentSystem->GetEmitter()->getMinMaxCurve(Emitter_ColorR);
			curve->SetCurveState(MinMaxCurve::Scalar);
			curve->SetScalar(mMinMaxColorR.Calculate(0));
			return;
		}
		mMinMaxColorA.SetCurveState(MinMaxCurve::Curve);
		mMinMaxColorR.SetCurveState(MinMaxCurve::Curve);
		mMinMaxColorG.SetCurveState(MinMaxCurve::Curve);
		mMinMaxColorB.SetCurveState(MinMaxCurve::Curve);
		Util::Array<Math::float2> _list;
		Util::Array<bool> _type;
		Math::MinMaxCurve* curve = mParentSystem->GetEmitter()->getMinMaxCurve(Emitter_ColorA);
		_list.Append(Math::float2(0,curve->Calculate(0)));
		_type.Append(true);
		_type.Append(true);
		_list.Append(Math::float2(1,mMinMaxColorA.Calculate(0)));
		mMinMaxColorA.SetCurveFromArray(_list,_type,_list,_type);
		
		curve = mParentSystem->GetEmitter()->getMinMaxCurve(Emitter_ColorR);
		_list.Clear();
		_list.Append(Math::float2(0,curve->Calculate(0)));
		_list.Append(Math::float2(1,mMinMaxColorR.Calculate(0)));
		mMinMaxColorR.SetCurveFromArray(_list,_type,_list,_type);
		
		curve = mParentSystem->GetEmitter()->getMinMaxCurve(Emitter_ColorG);
		_list.Clear();
		_list.Append(Math::float2(0,curve->Calculate(0)));
		_list.Append(Math::float2(1,mMinMaxColorG.Calculate(0)));
		mMinMaxColorG.SetCurveFromArray(_list,_type,_list,_type);

		curve = mParentSystem->GetEmitter()->getMinMaxCurve(Emitter_ColorB);
		_list.Clear();
		_list.Append(Math::float2(0,curve->Calculate(0)));
		_list.Append(Math::float2(1,mMinMaxColorB.Calculate(0)));
		mMinMaxColorB.SetCurveFromArray(_list,_type,_list,_type);
	}
}

