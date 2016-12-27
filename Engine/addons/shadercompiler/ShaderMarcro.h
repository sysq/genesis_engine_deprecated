#ifndef _SUBGPUPROGRAM_H_
#define _SUBGPUPROGRAM_H_

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

#include "util/dictionary.h"
#include "util/string.h"
#include "util/array.h"
#include "core/singleton.h"
#include "core/ptr.h"

namespace ShaderProgramCompiler
{
	class ShadingTemplateSetting;

	class ShaderMarcro : public Core::RefCounted
{
	__DeclareSubClass(ShaderMarcro, Core::RefCounted);

public:

	ShaderMarcro();

	~ShaderMarcro();

	void Init(const Util::Array<Util::String>& customMacros, const ShadingTemplateSetting* pSetting = NULL);

	void Add(const Util::String& name, bool bCustom = false);	

	void TurnOn(const Util::String& name);

	void TurnOff(const Util::String& name);

	void SetShaderMask(const uint mask);

	const uint GetShaderMask() const;

	const Util::Array<Util::String>& GetAllMarcroName() const;

	const Util::Array<Util::String>& GetBuiltInMacroName() const;

	uint CreateMask(const Util::String& name) const;

	void Reset();

	const Util::Dictionary<Util::String, uint>& GetShaderMarcroList() const;
	
	void Clone(const GPtr<ShaderMarcro>& source);

	void TurnOnLight();

public:

	static Util::String                  m_sLighting;
	static Util::String                  m_sLightMap;
	static Util::String                  m_sShadow;
	static Util::String                  m_sFog;
	static Util::String					 m_sParticleForce;
	static Util::String					 m_sParticleGravity;
	static Util::String					 m_sParticleMovement;
	static Util::String					 m_sParticleColor;
	static Util::String					 m_sParticleTexAnim;
	static Util::String					 m_sParticleScale;

protected:

	const bool _HasMacro(const Util::String& name) const;

	const IndexT _HasTurnOn(const Util::String& name) const;

private:

	uint        m_ShaderMask;

	Util::Dictionary<Util::String, uint> m_ShaderMarcoList;
	Util::Array<Util::String>            m_ShaderMarcroName;
	
	Util::Array<Util::String>            m_BuiltInMacroName;
};

inline void ShaderMarcro::SetShaderMask(const uint mask)
{
	m_ShaderMask = mask;
}

inline const uint ShaderMarcro::GetShaderMask() const
{
	return m_ShaderMask;
}

inline void ShaderMarcro::Reset()
{
	m_ShaderMask = 0;
}

inline const Util::Dictionary<Util::String, uint>& ShaderMarcro::GetShaderMarcroList() const
{
	return m_ShaderMarcoList;
}


}






#endif