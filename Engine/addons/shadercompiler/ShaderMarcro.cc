#include "stdneb.h"
#include "ShaderMarcro.h"
#include "ShaderCompilerSetting.h"
#include "ShadercompilerConfig.h"

namespace ShaderProgramCompiler
{

	__ImplementClass(ShaderMarcro,'SMRO',Core::RefCounted);
	Util::String ShaderMarcro::m_sLighting = "LIGHTING_ON";
	Util::String ShaderMarcro::m_sLightMap  = "LIGHTMAP_ON";
	Util::String ShaderMarcro::m_sShadow    = "SHADOW_ON";
	Util::String ShaderMarcro::m_sFog       = "FOG_ON";
	Util::String ShaderMarcro::m_sParticleForce = "PARTICLE_FORCE";
	Util::String ShaderMarcro::m_sParticleGravity = "PARTICLE_GRAVITY";
	Util::String ShaderMarcro::m_sParticleMovement = "PARTICLE_MOVEMENT";
	Util::String ShaderMarcro::m_sParticleColor = "PARTICLE_COLOR";
	Util::String ShaderMarcro::m_sParticleTexAnim = "PARTICLE_TEXANIM";
	Util::String ShaderMarcro::m_sParticleScale = "PARTICLE_SCALE";

	static Util::String g_NoMacro = "NO_ANY";

ShaderMarcro::ShaderMarcro()
	: m_ShaderMask(0)
{
	
}

ShaderMarcro::~ShaderMarcro()
{
	m_ShaderMarcoList.Clear();
	m_ShaderMarcroName.Clear();
}

void ShaderMarcro::Init(const Util::Array<Util::String>& customMacros, const ShadingTemplateSetting* pSetting)
{
	if (!customMacros.IsEmpty())
	{
		if (customMacros[0] == g_NoMacro)
		{
			return;
		}
	}


	if (pSetting != NULL)
	{

		Add(m_sLighting);
		
		if (pSetting->IsEnableLightMap())
		{
			Add(m_sLightMap);
		}
		
		if (ShaderCompilerConfig::shaderCompileType == ShaderCompilerConfig::D3D9_SHADER_COMPILER)
		{
			if (pSetting->IsCastShadow() && !pSetting->IsEnalbeAlphaBlend())
			{
				Add(m_sShadow);
			}
		}

		if (pSetting->IsEnableFog())
		{
			Add(m_sFog);
		}
	}

	for (IndexT i = 0; i < customMacros.Size(); ++i)
	{
		Add(customMacros[i], true);
	}
}

void ShaderMarcro::Add(const Util::String& name, bool bCustom)
{
	IndexT res = m_ShaderMarcoList.FindIndex(name);

	if (res == InvalidIndex)
	{
		SizeT mask = m_ShaderMarcoList.Size();
		m_ShaderMarcoList.Add(name, (uint)mask);

		if (!ShaderProgramCompiler::ShaderCompilerConfig::usePreCompileShader)
		{
			m_ShaderMarcroName.Append(name);

			if (!bCustom)
			{
				m_BuiltInMacroName.Append(name);
			}
		}

		
	}
}

const bool ShaderMarcro::_HasMacro(const Util::String& name) const
{
	IndexT res = m_ShaderMarcoList.FindIndex(name);

	if (res != InvalidIndex)
	{
		return true;
	} 
	else
	{
		return false;
	}
}

void ShaderMarcro::TurnOn(const Util::String& name)
{
	if (_HasMacro(name))
	{
		m_ShaderMask |= ( 1 << m_ShaderMarcoList[name] );
	}
}

void ShaderMarcro::TurnOff(const Util::String& name)
{
	if (_HasMacro(name))
	{
		m_ShaderMask &= ~( 1 << m_ShaderMarcoList[name] );
	}
}

const IndexT ShaderMarcro::_HasTurnOn(const Util::String& name) const
{
	if (_HasMacro(name))
	{
		return ( m_ShaderMask & (1 << m_ShaderMarcoList[name]) );
	}
	else
	{
		return 0;
	}
}

void ShaderMarcro::TurnOnLight()
{
	if (_HasTurnOn(m_sLightMap))
	{
		return;
	}

	TurnOn(m_sLighting);
	
}

const Util::Array<Util::String>& ShaderMarcro::GetAllMarcroName() const
{
	return m_ShaderMarcroName;
}

const Util::Array<Util::String>& ShaderMarcro::GetBuiltInMacroName() const
{
	return m_BuiltInMacroName;
}

uint ShaderMarcro::CreateMask(const Util::String& name) const
{
	if (_HasMacro(name))
	{
		return ( 1 << m_ShaderMarcoList[name] );
	}
	else
	{
		return 0;
	}
}

void ShaderMarcro::Clone(const GPtr<ShaderMarcro>& source)
{
	m_ShaderMask = source->GetShaderMask();

	m_ShaderMarcoList = source->GetShaderMarcroList();

	m_ShaderMarcroName = source->GetAllMarcroName();
}


}