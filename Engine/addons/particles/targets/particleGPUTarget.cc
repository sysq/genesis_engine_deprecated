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
#include "particles/targets/particleGPUTarget.h"
#include "particles/particlesystem.h"
#include "graphicsystem/GraphicSystem.h"
#include "particles/particlepool.h"

namespace Particles
{
	using namespace RenderBase;
	__ImplementClass(Particles::ParticleGPUTarget, CPFCC::TARGET_GPU, Particles::ParticleTarget );

	ParticleGPUTarget::ParticleGPUTarget()
	{

	}
	//------------------------------------------------------------------------
	ParticleGPUTarget::~ParticleGPUTarget()
	{

	}
	//------------------------------------------------------------------------
	void  
		ParticleGPUTarget::_updateTarget( GPtr<ParticlePool>& pool, Timing::Time frameTime)
	{
		if ( !mNeedPrimitive )
			return;
		if(!mParentSystem)
			return;
		float rate = mParentSystem->GetEmitter()->GetGPUParticleRate();
		float life = mParentSystem->GetEmitter()->GetGPUParticleLife();
		int quato = (int)(rate*life);

		if(mPrimitiveHandle.IsValid())
		{
			Graphic::GraphicSystem::Instance()->RemovePrimitive(mPrimitiveHandle);
			mPrimitiveHandle = NULL;
		}
		if(quato <= 0)
		{
			mPrimDirty = true;
			return;
		}
		mParentSystem->InitShaderParam();

		Graphic::VertexBufferData2 vbd2;
		Graphic::IndexBufferData2 ibd2;
		/*
		_initSpriteGPUVertexBuffer(vbd2, quato);
		dvb.SetSize(quato*sizeof(SpriteGPUParticleVertex));
		SpriteGPUParticleVertex* particleVertex = dvb.GetBufferPtr<SpriteGPUParticleVertex>();
		mParentSystem->SetupSpriteGPUParticles(particleVertex,quato);
		mPrimitiveHandle = Graphic::GraphicSystem::Instance()->CreatePrimitiveHandle(&vbd2);
		mActiveVertexCount = quato;
		Graphic::GraphicSystem::Instance()->UpdatePrimitiveHandle(mPrimitiveHandle, &dvb);
		mPrimDirty = true;
		mNeedPrimitive = false;
		*/

		mActiveVertexCount = 4*quato;
		mActiveIndexCount = 6*quato;

		vbd2.Setup(mActiveVertexCount,sizeof(BoardGPUParticleVertex),
			RenderBase::BufferData::Static,RenderBase::PrimitiveTopology::TriangleList,true);

		ibd2.Setup(mActiveIndexCount, RenderBase::BufferData::Static, RenderBase::IndexBufferData::Int16, true);

		BoardGPUParticleVertex* particleVertex = vbd2.GetBufferPtr<BoardGPUParticleVertex>();
		ushort* indicies = ibd2.GetBufferPtr<ushort>();

		mParentSystem->SetupBoardGPUParticles(particleVertex,indicies,quato);
		
		_initBoardGPUVertexBuffer(vbd2,mActiveVertexCount);

		if(mPrimitiveHandle.IsValid())
		{
			Graphic::GraphicSystem::Instance()->RemovePrimitive(mPrimitiveHandle);
		}
		mPrimitiveHandle = Graphic::GraphicSystem::Instance()->CreatePrimitiveHandle(&vbd2,&ibd2);


		mPrimDirty = true;
		mNeedPrimitive = false;
	}
}