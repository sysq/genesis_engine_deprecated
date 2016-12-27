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

namespace Particles
{
	using namespace Serialization;
	const char* s_GPUParticleType = "GPUParticleType";
	const char* s_GPUAffectElem = "GPUElem";
	class ParticleGPUTargetSerialization
	{
	public:
		//------------------------------------------------------------------------
		void Load( ParticleGPUTarget* obj,  SVersion ver, SerializeReader* pReader )
		{
			if ( 1 == ver )
			{
				Load_1(obj, pReader);
				return;
			}
			else if( 2 == ver)
			{
				//nothing;
				return;
			}
			n_error(" %s Load unknown version.\n", obj->GetClassName().AsCharPtr() );
		}
		//------------------------------------------------------------------------
		void Load_1(ParticleGPUTarget* obj, SerializeReader* pReader)
		{
			int _type;
			pReader->SerializeInt(s_GPUParticleType,_type);
		//	obj->SetGPUParticleType(ParticleGPUTarget::GPUParticleType(_type));

			Math::float4 _affect;
			pReader->SerializeFloat4(s_GPUAffectElem,_affect);
		//	obj->SetForce(_affect.x(),_affect.y(),_affect.z());
		//	obj->SetColorAlpha(_affect.w());
		}
		
		//------------------------------------------------------------------------
		void Save( const ParticleGPUTarget* obj, SerializeWriter* pWriter )
		{
		//	pWriter->SerializeInt(s_GPUParticleType,(int)obj->GetGPUParticleType());
		//	pWriter->SerializeFloat4(s_GPUAffectElem,obj->GetAffectEle());
		}
	};

	//------------------------------------------------------------------------
	// @ISerialization::GetVersion. when change storage, must add SerializeVersion count
	SVersion ParticleGPUTarget::GetVersion() const
	{
		return 2;	
	}

	//------------------------------------------------------------------------
	// @ISerialization::Load 
	void ParticleGPUTarget::Load( SVersion ver, SerializeReader* pReader, const Serialization::SerializationArgs* args )
	{
		pReader->SerializeSuper<Super>(this, args);
		ParticleGPUTargetSerialization Serialize;
		Serialize.Load( this, ver, pReader );
	}

	//------------------------------------------------------------------------
	// @ISerialization::Save
	void ParticleGPUTarget::Save( SerializeWriter* pWriter ) const
	{
		pWriter->SerializeSuper<Super>(this);
		ParticleGPUTargetSerialization Serialize;
		Serialize.Save( this, pWriter );
	}
	//------------------------------------------------------------------------
	void ParticleGPUTarget::CopyFrom( const ParticleTargetPtr& target )
	{
		Super::CopyFrom( target );
		n_assert( target->GetRtti() == this->GetRtti() ); 
		GPtr<ParticleGPUTarget> source = target.downcast<ParticleGPUTarget>();
	}

}