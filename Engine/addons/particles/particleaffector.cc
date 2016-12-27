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
#include "particles/particleaffector.h"
#include "particles/particlesystem.h"
#include "particles/particle.h"

namespace Particles
{
	__ImplementClass(Particles::ParticleAffector, CPFCC::AFFECTOR_DEFAULT, Core::RefCounted)

		GPtr<ParticleAffector> ParticleAffector::NullAffector(NULL);
	//------------------------------------------------------------------------
	ParticleAffector::ParticleAffector()
		: mParentSystem(NULL)
		, mIsActive(false)
		, mName("Affector")
		, mAffectType(ParticleAffector::UnKnown)
		, mEnable(true)
	{

	}
	//------------------------------------------------------------------------
	ParticleAffector::~ParticleAffector()
	{

	}
	//------------------------------------------------------------------------
	void 
		ParticleAffector::_onActivate(void)
	{
		n_assert( !mIsActive );
		mIsActive = true;
	}
	//------------------------------------------------------------------------
	void 
		ParticleAffector::_onDeactivate(void)
	{
		n_assert( mIsActive );
		mIsActive = false;
	}
	//------------------------------------------------------------------------
	void 
		ParticleAffector::_preProcessParticles(void)
	{
	}
	//------------------------------------------------------------------------
	void 
		ParticleAffector::_processParticle( Particle* particle, bool firstParticle)
	{
		if (firstParticle)
		{
			_firstParticle( particle);
		}
		_affect( particle);
	}
	//------------------------------------------------------------------------
	void 
		ParticleAffector::_postProcessParticles()
	{

	}
	//------------------------------------------------------------------------
	void 
		ParticleAffector::_firstParticle( Particle* particle )
	{

	}
	//------------------------------------------------------------------------
	void 
		ParticleAffector::_affect(Particle* particle )
	{
		n_assert( particle );
		_affectPositon(particle);
	}
	//------------------------------------------------------------------------
	void 
		ParticleAffector::_affectPositon(Particle* particle)
	{
		particle->mPosition += (particle->mDirection * (Math::scalar)mParentSystem->GetCurrentFrameTime() );
	}
	//--------------------------------------------------------------------------------
	Math::MinMaxCurve*  ParticleAffector::getMinMaxCurve(ParticleCurveType pct)
	{
		return NULL;
	}
	//---------------------------------------------------------------------------------
	void ParticleAffector::_switchParticleType(bool _isMobile)
	{

	}
	//---------------------------------------------------------------------------------
	void ParticleAffector::SetShaderParam()
	{

	}
	//---------------------------------------------------------------------------------
	void ParticleAffector::InitShaderParam()
	{

	}
	//-----------------------------------------------------------------------------------
	void ParticleAffector::SetShaderMask(const GPtr<ShaderProgramCompiler::ShaderMarcro>& pMarcro)
	{

	}
}
