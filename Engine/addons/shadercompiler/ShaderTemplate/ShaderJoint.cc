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
#include "ShaderJoint.h"
#include "shadercompiler/Utility/ShaderCompilerUtil.h"
#include "shadercompiler/ShaderCompilerSetting.h"
#include "shadercompiler/Compilers/MojoShaderTool.h"
#include "shadercompiler/ShadercompilerConfig.h"
#ifdef _WIN32
#include "Cg/cg.h"
#endif

namespace ShaderProgramCompiler
{

const char g_sSimpleLight[] = "float4 simplelight(TEMPLATE_OUT sin, float3 viewDir, float lightAtten)\n"
	"{\n"
	"float4 diffuseColor = float4(0.0,0.0,0.0,0.0);\n"
	"float4 specularColor = float4(0.0,0.0,0.0,0.0);\n"
	"float4 c = float4(0,0,0,1);\n"
	"SimpleBlinnPhong(sin.Normal,viewDir,1.0,diffuseColor,specularColor);\n"
	"c = diffuseColor * sin.Diffuse;\n"
	"return c;\n"
	"}\n";

ShaderJoint::ShaderJoint()
{
	
}

ShaderJoint::~ShaderJoint()
{

}

void ShaderJoint::Generate(CommonShaderSetting* pCommon, ShadingTemplateSetting* pSegment, const Util::String& renderState, const Util::String& source)
{

	m_sJointedSource += source;

	IndexT firstStructIndex = m_sJointedSource.FindStringIndex("struct",0);

	Util::String templateOutPutStr = "\n struct TEMPLATE_OUT\n ";
	templateOutPutStr += "{\n";
	templateOutPutStr += "\tfloat3 Emission;\n";
	templateOutPutStr += "\tfloat4 Diffuse;\n";
	templateOutPutStr += "\tfloat3 Normal;\n";
	templateOutPutStr += "\tfloat  Alpha;\n";
	templateOutPutStr += "\tfloat4 Specular;\n";
	templateOutPutStr += "\tfloat  Shininess;\n";


	if (pSegment->IsEnableDirLightMap())
	{
		templateOutPutStr += "\tfloat3x3 TangentToWorld;";
	}

	if (pSegment->IsEnableVertexLight())
	{
		templateOutPutStr += "\tfloat3 VertexLightDiffuse;\n";
		templateOutPutStr += "\tfloat3 VertexLightSpecular;\n";
	}

	templateOutPutStr += "};\n";

	if (firstStructIndex == InvalidIndex)
	{
		SYS_EXCEPT(Exceptions::ShaderException, 
			STRING_FORMAT("Shading Segment must define a struct INPUT!\n", ""),
			GET_FUNCTION_NAME()
			);
		return;
	}

	m_sJointedSource = m_sJointedSource.InsertRange(firstStructIndex-1, templateOutPutStr);

	if (pCommon->HasEntry(CommonShaderSetting::Vertex))
	{
		Util::String vsOutPutStr = "\n struct VS_OUTPUT\n ";
		vsOutPutStr += "{\n";
		vsOutPutStr += "  float4 Position : POSITION;\n";
		vsOutPutStr += "  float2 Uv     : TEXCOORD0;\n";
		vsOutPutStr += "  float3 Normal : TEXCOORD1;\n";
		vsOutPutStr += "  float4 Tangent   : TEXCOORD2;\n";
		vsOutPutStr += "  float3 ViewDir	  : TEXCOORD3;\n";
		vsOutPutStr += "  float4 ClipSpacePos : TEXCOORD4;\n";
		vsOutPutStr += "  float3 CameraSpacePos : TEXCOORD5;\n";
		vsOutPutStr += "  float4 Color : TEXCOORD6;\n";
		vsOutPutStr += "  float4 ExternUvs : TEXCOORD7;\n";

		vsOutPutStr += "#if LIGHTMAP_ON\n";
		vsOutPutStr += "float2 LightmapUV : TEXCOORD8;\n";
		vsOutPutStr += "#endif\n";

		vsOutPutStr += "};\n";

		m_sJointedSource = m_sJointedSource.InsertRange(firstStructIndex-1, vsOutPutStr);

	}

	MojoShaderTool::Instance()->MojoJointParse(this, m_sJointedSource, pCommon->GetEntryName(CommonShaderSetting::Template));

	_ParseVsOutput(pCommon, pSegment);

	_GenerateForwardPass(pCommon, pSegment);

	_GenerateDepthPass(pCommon, pSegment, renderState);

 if ( ShaderCompilerConfig::shaderCompileType == ShaderCompilerConfig::D3D9_SHADER_COMPILER )
 {
	 _GenerateShadowPass(pCommon, pSegment, renderState);
 }
	


	

}

void ShaderJoint::_ParseVsOutput(const CommonShaderSetting* pCommon, const ShadingTemplateSetting* pSegment)
{
#ifdef _WIN32
	static Util::String includeContent;
	if (includeContent.IsEmpty())
	{
		ShaderUtil::ReadText("sys:common.wjh", includeContent);
	}
	

	Util::String testCode = "#define D3D9\n" + includeContent + m_sJointedSource + g_sSimpleLight;

	testCode += "float4 psMain (" + m_CustomFunc.paramInputType + " p) : COLOR\n";
	testCode += "{\n";
	testCode += "TEMPLATE_OUT o;\n";
	testCode += pCommon->GetEntryName(CommonShaderSetting::Template) + "(p, o);\n";

	testCode += "float4 color = float4(0,0,0,0);\n";
	testCode += "color = simplelight(o, float3(0,1,0), 1) + float4(o.Emission, 0.0);\n";
	testCode += "return color;\n";
	testCode += "}\n";


	CGcontext ctx = cgCreateContext();

	CGprogram pProgram = cgCreateProgram( ctx, CG_SOURCE, testCode.AsCharPtr(), CG_PROFILE_PS_3_0, "psMain", NULL );

	CGerror cgError = cgGetError();

	if (cgError != CG_NO_ERROR)
	{
		const char* pLastList = cgGetLastListing(ctx);
		
		SYS_EXCEPT(Exceptions::ShaderException, 
			STRING_FORMAT("Compile Shader Error : %s \n.", pLastList),
			GET_FUNCTION_NAME()
			);
	}

	Util::Array<Util::String> inputName;

	CGparameter param = cgGetFirstLeafParameter (pProgram, CG_PROGRAM);
	
	while (param != NULL)
	{
		CGbool bRef       = cgIsParameterReferenced(param);
		const char* pName = cgGetParameterName (param); 
		if (bRef)
		{
			CGenum dir = cgGetParameterDirection (param);

			if (dir == CG_IN)
			{
				inputName.Append(pName);
			}
			
		}

		param = cgGetNextLeafParameter (param);
	}

	SizeT nCount = inputName.Size();

	for (IndexT i = 0; i < nCount; ++i)
	{
		const Util::String& name = inputName[i];
		
		if (ShaderUtil::HaveStringAtEnd(name, "_Normal"))
		{
			m_CustomFunc.vsOutput.bNormal = true;
		}
		if (ShaderUtil::HaveStringAtEnd(name, "_Tangent"))
		{
			m_CustomFunc.vsOutput.bTangent = true;
		}
		if (ShaderUtil::HaveStringAtEnd(name, "_ClipSpacePos") )
		{
			m_CustomFunc.vsOutput.bClipSpacePos = true;
		}
		if (ShaderUtil::HaveStringAtEnd(name, "_CameraSpacePos") )
		{
			m_CustomFunc.vsOutput.bCameraSpacePos = true;
		}
		if (ShaderUtil::HaveStringAtEnd(name, "_Color"))
		{
			m_CustomFunc.vsOutput.bColor = true;
		}
		if (ShaderUtil::HaveStringAtEnd(name, "_UV2") || ShaderUtil::HaveStringAtEnd(name, "_UV3"))
		{
			m_CustomFunc.vsOutput.bExternUvs = true;
		}
		if (ShaderUtil::HaveStringAtEnd(name, "_ViewDir"))
		{
			m_CustomFunc.vsOutput.bViewDir = true;
		}
		
		
	}

	cgDestroyProgram(pProgram);
	cgDestroyContext(ctx);
#endif
	
}

void ShaderJoint::_GenerateForwardPass(const CommonShaderSetting* pCommon, const ShadingTemplateSetting* pSegment)
{
	Util::String vsSeg;

	Util::String code = m_sJointedSource;

	if (!pCommon->HasEntry(CommonShaderSetting::Vertex))
	{
		_CreateVsOutput(pSegment, vsSeg, Forward);
		vsSeg += "};\n";

		code += vsSeg;
		_CreateVsSegment(pCommon, pSegment, Forward, code);
		code += "  return o;\n";
		code += "}\n";
	}
	

	_CreatePsSegment(pCommon, pSegment, Forward, code);

	code += "float4 diffuseColor = float4(0.0,0.0,0.0,0.0);\n";
	code += "float4 specularColor = float4(0.0,0.0,0.0,0.0);\n";

	code += "float percentLit = 1.0f;\n";

	if (pSegment->IsCastShadow() && !pSegment->IsEnalbeAlphaBlend())
	{
		code += "#if defined(SHADOW_ON) && defined(D3D9)\n";
		code += "float2 hPos = IN.ClipSpacePos.xy/IN.ClipSpacePos.w;\n";
		code += "float lit = tex2D(g_LightLitMap,getUVfromscreenspace(hPos)).r;\n";
		code += "percentLit = lit;\n";
		code += "#endif\n";
	}

	Util::String tmpStr = pSegment->GetLightingType();

	//¹âÕÕÆ´½Ó
	{
		if(tmpStr == "default")
		{
			if (pSegment->IsEnableVertexLight())
			{
				code += "float4 color =  float4(o.VertexLightDiffuse, 1.0f) * o.Diffuse +  float4(o.VertexLightSpecular, 1.0f) * o.Specular;\n";
			} 
			else
			{
				code += "float3 viewDir = float3(0,0,1);\n";
				code += "#if defined(LIGHTING_ON)\n";
				code += "viewDir = IN.ViewDir;\n";
				code += "#endif\n";

				code += "LightingBlinnPhong(o.Normal,viewDir,o.Shininess,diffuseColor,specularColor,percentLit);\n";
				code += "float4 color =  diffuseColor * o.Diffuse + specularColor * o.Specular;\n";
			}

		}
		else
		{
			if (!pSegment->IsEnableVertexLight())
			{
				code += "float4 color = float4(0,0,0,0);\n";
				code += "float3 viewDir = float3(0,0,1);\n";
				code += "#if defined(LIGHTING_ON)\n";
				code += "viewDir = IN.ViewDir;\n";
				code += "#endif\n";
				code += "color = " + pSegment->GetLightingType() + "( o , viewDir, percentLit );\n";
			}
			else
			{
				code += "float4 color =  float4(o.VertexLightDiffuse, 1.0f) * o.Diffuse +  float4(o.VertexLightSpecular, 1.0f) * o.Specular;\n";
			}

		}
	}
	
	// lightmap support
	// support direction lightmap
	if(pSegment->IsEnableDirLightMap())
	{
		// xy for scale, zw for offset
		code += "#if LIGHTMAP_ON\n";
		code += "float4 lightmapColor_Dir = tex2D(g_Lightmap, IN.LightmapUV);\n";
		code += "float4 lightmapScale_Dir = tex2D(g_Lightmap2, IN.LightmapUV);\n";

		code += "if (g_UseLM.r > 1.5f)\n";
		code += "{\n";
		code += "lightmapColor_Dir.xyz = DecodeLightmap(lightmapColor_Dir);\n";
		code += "lightmapScale_Dir.xyz = DecodeLightmap(lightmapScale_Dir);\n";

		code += "Forward_Shading_DirLightmap(lightmapColor_Dir.xyz,lightmapScale_Dir.xyz, o.TangentToWorld, o.Normal, IN.ViewDir, o.Shininess, specularColor, percentLit );\n";

		code += "color.xyz = o.Diffuse.xyz * lightmapColor_Dir.xyz;\n";
		code += "color.xyz += o.Specular.xyz * specularColor.xyz;\n";
		code += "}\n";
		code += "#endif\n";
	}
	else if (pSegment->IsEnableLightMap())
	{
		code += "#if LIGHTMAP_ON\n";
		code += "float4 lightmapColor = tex2D(g_Lightmap, IN.LightmapUV);\n";		
		code += "color.xyz = o.Diffuse.xyz * DecodeLightmap(lightmapColor)*g_UseLM.r*percentLit;\n";
		code += "#endif\n";
	}

	// add emission
	code += "color.xyz += o.Emission.rgb;\n";

	//fog support
	{
		if (pSegment->IsEnableFog())
		{
			code += "#if FOG_ON\n";
			code += "color = ComputeHeightFog (IN.CameraSpacePos,IN.ViewDir,color);\n";
			code += "#endif\n";
		}
	}

	code += "color.a = o.Alpha;\n";
	code += "return color;\n";
	code += "}\n";

	_GenerateFinalCode(pCommon, pSegment, Forward, code);
}

void ShaderJoint::_GenerateDepthPass(const CommonShaderSetting* pCommon, const ShadingTemplateSetting* pSegment, const Util::String& renderState)
{
	Util::String result;
	_ReGenRenderState(pSegment, renderState, Depth, result);

	m_PassRenderState.Add(Depth, result);

	Util::String vsSeg;

	Util::String code = m_sJointedSource;

	if (!pCommon->HasEntry(CommonShaderSetting::Vertex))
	{
		_CreateVsOutput(pSegment, vsSeg, Depth);
		vsSeg += "};\n";

		code += vsSeg;
		_CreateVsSegment(pCommon, pSegment, Depth, code);
		code += "  return o;\n";
		code += "}\n";
	}

	_CreatePsSegment(pCommon, pSegment, Depth, code);

	if (pSegment->IsEnalbeAlphaBlend())
	{
		code += "  clip(-1);\n";
	}

	code += "  float4 color = -IN.CameraSpacePos.z / g_CameraParam.y;\n";//g_CameraPos.w
	code += "  color.b = 0.5f;\n";
	code += "  color.a = 1.0f;\n";
	code += "#ifdef GLES \n";
	code += "color = packFloatToVec4i(color.r);\n";
	code += "#endif \n";
	code += "  return color;\n";
	code += "}\n";

	_GenerateFinalCode(pCommon, pSegment, Depth, code);
}

void ShaderJoint::_GenerateShadowPass(const CommonShaderSetting* pCommon, const ShadingTemplateSetting* pSegment, const Util::String& renderState)
{
	Util::String result;
	_ReGenRenderState(pSegment, renderState, Shadow, result);

	m_PassRenderState.Add(Shadow, result);

	Util::String vsSeg;

	Util::String code = m_sJointedSource;

	if (!pCommon->HasEntry(CommonShaderSetting::Vertex))
	{
		_CreateVsOutput(pSegment, vsSeg, Shadow);
		vsSeg += "};\n";

		code += vsSeg;
		_CreateVsSegment(pCommon, pSegment, Shadow, code);
		code += "  return o;\n";
		code += "}\n";
	}

	_CreatePsSegment(pCommon, pSegment, Shadow, code);

	if (pSegment->IsEnalbeAlphaBlend() || !pSegment->IsGenShadow())
	{
		code += "  clip(-1);\n";
	}

	code += "  float2 color;\n";
	code += "  color.r = (-IN.CameraSpacePos.z - g_CameraParam.x) / (g_CameraParam.y - g_CameraParam.x); \n";//g_CameraPos.w
	//depth bias
	code += "  float bias = g_ShadowMapSize.z * max(abs(ddx(color.r)), abs(ddy(color.r)));";
	code += "  color.r += bias + 0.00002f;"; 
	//
	code += "  color.g = color.r*color.r;\n";
	code += "  return float4(color,0.0,1.0);\n";
	code += "}\n";

	_GenerateFinalCode(pCommon, pSegment, Shadow, code);
}

void ShaderJoint::_CreateVsOutput(const ShadingTemplateSetting* pSegment, Util::String& code, PassType type)
{
	code += "struct VS_OUTPUT {\n";
	code += "  float4 Position : POSITION;\n";
	code += "  float2 Uv     : TEXCOORD0;\n";

	IndexT nTexCoord = 1;
	Util::String texCoord;

	if (m_CustomFunc.vsOutput.bNormal)
	{
		texCoord.Clear();
		texCoord.Format("TEXCOORD%d;", nTexCoord);

		if (pSegment->IsEnableDirLightMap())
		{
			code += "  float3 Normal : " + texCoord + "\n";
		}
		else
		{
			code += "#if defined(LIGHTING_ON)\n";
			code += "  float3 Normal : " + texCoord + "\n";
			code += "#endif\n";
		}
		
		nTexCoord++;
	}
	
	if (m_CustomFunc.vsOutput.bTangent)
	{
		texCoord.Clear();
		texCoord.Format("TEXCOORD%d;", nTexCoord);
		code += "  float4 Tangent   : " + texCoord + "\n";
		nTexCoord++;
	}

	if (type == Forward)
	{

		if (m_CustomFunc.vsOutput.bViewDir)
		{
			texCoord.Clear();
			texCoord.Format("TEXCOORD%d;", nTexCoord);
			code += "  float3 ViewDir	  : " + texCoord +"\n";
			nTexCoord++;
		}
		else
		{
			texCoord.Clear();
			texCoord.Format("TEXCOORD%d;", nTexCoord);
			if (pSegment->IsEnableDirLightMap())
			{
				code += "  float3 ViewDir	  : " + texCoord +"\n";
			}
			else
			{
				code += "#if defined(LIGHTING_ON) || defined(FOG_ON)\n";
				code += "  float3 ViewDir	  : " + texCoord +"\n";
				code += "#endif\n";
			}
			
			nTexCoord++;
		}

		if (m_CustomFunc.vsOutput.bClipSpacePos)
		{
			texCoord.Clear();
			texCoord.Format("TEXCOORD%d;", nTexCoord);
			code += "  float4 ClipSpacePos : " + texCoord +"\n";
			nTexCoord++;
		}
		else
		{
			texCoord.Clear();
			texCoord.Format("TEXCOORD%d;", nTexCoord);
			code += "#if SHADOW_ON \n";
			code += "  float4 ClipSpacePos : " + texCoord +"\n";
			code += "#endif\n";
			nTexCoord++;
		}

		if (m_CustomFunc.vsOutput.bCameraSpacePos)
		{
			texCoord.Clear();
			texCoord.Format("TEXCOORD%d;", nTexCoord);
			code += "  float3 CameraSpacePos : " + texCoord +"\n";
			nTexCoord++;
		}
		else
		{
			texCoord.Clear();
			texCoord.Format("TEXCOORD%d;", nTexCoord);
			code += "#if FOG_ON \n";
			code += "  float3 CameraSpacePos : " + texCoord +"\n";
			code += "#endif\n";
			nTexCoord++;
		}
	}

	if (type == Depth || type == Shadow)
	{
		texCoord.Clear();
		texCoord.Format("TEXCOORD%d;", nTexCoord);
		code += "  float3 CameraSpacePos : " + texCoord +"\n";
		nTexCoord++;
	}
	
	if (m_CustomFunc.vsOutput.bColor)
	{
		texCoord.Clear();
		texCoord.Format("TEXCOORD%d;", nTexCoord);
		code += "  float4 Color : " + texCoord + "\n";
		nTexCoord++;
	}
	
	if (m_CustomFunc.vsOutput.bExternUvs)
	{
		texCoord.Clear();
		texCoord.Format("TEXCOORD%d;", nTexCoord);
		code += "  float4 ExternUvs : " + texCoord + "\n";
		nTexCoord++;
	}

	code += "#if LIGHTMAP_ON\n";
	texCoord.Clear();
	texCoord.Format("TEXCOORD%d;", nTexCoord);
	code +=  "  float2 LightmapUV : " + texCoord + "\n";
	code += "#endif\n";
	
}

void ShaderJoint::_CreateVsSegment(const CommonShaderSetting* pCommon, const ShadingTemplateSetting* pSegment, const ShaderJoint::PassType& type, Util::String& code)
{	
	code += "VS_OUTPUT vsMain (VS_INPUT v) {\n";
	code += "  VS_OUTPUT o;\n";
	code += "o.Position = mul( g_ModelViewProj , float4(v.vertex,1.0f) );\n";

	if (m_CustomFunc.vsOutput.bNormal)
	{
		if (pSegment->IsEnableDirLightMap())
		{
			code += "o.Normal = v.normal;\n";
		}
		else
		{
			code += "#if defined(LIGHTING_ON)\n";
			code += "o.Normal = v.normal;\n";
			code += "#endif\n";
		}
	}
	
	if (type == Forward)
	{
		if (pSegment->IsEnableDirLightMap())
		{
			code += "o.ViewDir = g_CameraPos.xyz - mul( g_Model ,float4(v.vertex,1.0f) ).xyz;\n";
		}
		else
		{
			code += "#if defined(LIGHTING_ON) || defined(FOG_ON)\n";
			code += "o.ViewDir = g_CameraPos.xyz - mul( g_Model ,float4(v.vertex,1.0f) ).xyz;\n";
			code += "#endif\n";
		}

		

		if (m_CustomFunc.vsOutput.bClipSpacePos)
		{
			code += "o.ClipSpacePos = o.Position;\n";
		}
		else
		{
			code += "#if defined(SHADOW_ON) && defined(D3D9)\n";
			code +=  "o.ClipSpacePos = o.Position;\n";
			code += "#endif\n";
		}

		if (m_CustomFunc.vsOutput.bCameraSpacePos)
		{
			code += "o.CameraSpacePos = mul( g_ModelView , float4(v.vertex,1.0f) ).xyz;\n";
		}
		else
		{
			code += "#if FOG_ON\n";
			code += "o.CameraSpacePos = mul( g_ModelView , float4(v.vertex,1.0f) ).xyz;\n";
			code += "#endif\n";
		}
		
	}
	
	code += "o.Uv = v.texcoord;\n";	
	
	if (type == Depth || type == Shadow)
	{
		code += "float4 viewpos = mul( g_ModelView , float4(v.vertex,1.0f) );\n"; 
		code += "o.CameraSpacePos = viewpos.xyz;\n";
	}

	if (m_CustomFunc.vsOutput.bColor)
	{
		code += "o.Color = v.color;\n";
	}
	
	
	if (pSegment->IsLightMapUseFirstUV())
	{
		code += "#if LIGHTMAP_ON\n";
		code += "o.LightmapUV.xy = v.texcoord.xy * g_LMTexParam.xy + g_LMTexParam.zw;;\n";
		code += "#endif\n";
	}
	else
	{
		code += "#if LIGHTMAP_ON\n";
		code += "o.LightmapUV.xy = v.texcoord1.xy * g_LMTexParam.xy + g_LMTexParam.zw;;\n";
		code += "#endif\n";
	}


	if (m_CustomFunc.vsOutput.bExternUvs)
	{
		code += "o.ExternUvs.xy = v.texcoord1.xy;\n";
	}

	if (m_CustomFunc.vsOutput.bTangent)
	{
		code += "o.Tangent = v.tangent;\n";
	}
}

void ShaderJoint::_CreatePsSegment(const CommonShaderSetting* pCommon, const ShadingTemplateSetting* pSegment, const ShaderJoint::PassType& type, Util::String& code)
{
	if (pSegment->IsEnalbeAlphaTest())
	{
		code += "float " + pSegment->GetAlphaTestValue() + ";\n";
	}

	SizeT nCount = m_CustomFunc.funcInputParams.Size();

	for (IndexT i = 0; i < nCount; ++i)
	{
		if (ShaderUtil::HaveStringAtEnd(m_CustomFunc.funcInputParams[i].identifier, "_UV"))
		{
			Util::String texOffestScale;
			texOffestScale.Clear();
			texOffestScale.Format("float4 %s_OffsetScale;\n", m_CustomFunc.funcInputParams[i].identifier.AsCharPtr());
			code += texOffestScale;
		}
	}
	
	if (type == Forward)
	{
		code += "float4 psMain (VS_OUTPUT IN) : COLOR {\n";
	}
	else
	{
		code += "float4 psMain (VS_OUTPUT IN) : COLOR {\n";
	}
	

	code += "  INPUT templateParam;\n";

	code += "  " + m_CustomFunc.paramOutputType + " o;\n";

	{
		Util::String tmp;
		for (IndexT i = 0; i < nCount; ++i)
		{
			const FunctionParameter& fp    = m_CustomFunc.funcInputParams[i];
			const Util::String& identifier = fp.identifier;

			if (ShaderUtil::HaveStringAtEnd(identifier, "_UV"))
			{
				tmp.Clear();				
				tmp.Format("templateParam.%s = IN.Uv * %s_OffsetScale.zw + %s_OffsetScale.xy;\n", identifier.AsCharPtr(),
					identifier.AsCharPtr(), identifier.AsCharPtr());
				code += tmp;
			}
			else if (ShaderUtil::HaveStringAtEnd(identifier, "_UV2"))
			{
				tmp.Clear();
				if (m_CustomFunc.vsOutput.bExternUvs)
				{
					tmp.Format("templateParam.%s = IN.ExternUvs.xy;\n", identifier.AsCharPtr());
				}
				else
				{
					Util::String sTemp = fp.userTypeName + "(1)";
					tmp.Format("templateParam.%s = %s;\n", identifier.AsCharPtr(), sTemp.AsCharPtr());
				}
				code += tmp;
			}
			else if (ShaderUtil::HaveStringAtEnd(identifier, "_UV3"))
			{
				tmp.Clear();
				if (m_CustomFunc.vsOutput.bExternUvs && type == Forward)
				{
					tmp.Format("templateParam.%s = IN.ExternUvs.zw;\n", identifier.AsCharPtr());
				}
				else
				{
					Util::String sTemp = fp.userTypeName + "(1)";
					tmp.Format("templateParam.%s = %s;\n", identifier.AsCharPtr(), sTemp.AsCharPtr());
				}
				
				code += tmp;
			}
			else if (ShaderUtil::HaveStringAtEnd(identifier, "_Normal"))
			{
				tmp.Clear();
				if (m_CustomFunc.vsOutput.bNormal && type == Forward)
				{
					tmp.Format("templateParam.%s = IN.Normal;\n", identifier.AsCharPtr());

					if (pSegment->IsEnableDirLightMap())
					{
						code += tmp;
					}
					else
					{
						code += "#if defined(LIGHTING_ON)\n";
						code += tmp;
						code += "#endif\n";
					}
				}
				else
				{
					Util::String sTemp = fp.userTypeName + "(1)";
					tmp.Format("templateParam.%s = %s;\n", identifier.AsCharPtr(), sTemp.AsCharPtr());
					code += tmp;
				}
				
			}
			else if (ShaderUtil::HaveStringAtEnd(identifier, "_Tangent"))
			{
				tmp.Clear();
				if (m_CustomFunc.vsOutput.bTangent && type == Forward)
				{
					tmp.Format("templateParam.%s = IN.Tangent;\n", identifier.AsCharPtr());
				}
				else
				{
					Util::String sTemp = fp.userTypeName + "(1)";
					tmp.Format("templateParam.%s = %s;\n", identifier.AsCharPtr(), sTemp.AsCharPtr());
				}
				code += tmp;
			}
			else if (ShaderUtil::HaveStringAtEnd(identifier, "_Color"))
			{
				tmp.Clear();
				if (m_CustomFunc.vsOutput.bColor && type == Forward)
				{
					tmp.Format("templateParam.%s = IN.Color;\n", identifier.AsCharPtr());
				}
				else
				{
					Util::String sTemp = fp.userTypeName + "(1)";
					tmp.Format("templateParam.%s = %s;\n", identifier.AsCharPtr(), sTemp.AsCharPtr());
				}
				code += tmp;
			}
			else if (ShaderUtil::HaveStringAtEnd(identifier, "_CameraSpacePos"))
			{
				tmp.Clear();
				if (m_CustomFunc.vsOutput.bCameraSpacePos && type == Forward)
				{
					tmp.Format("templateParam.%s = IN.CameraSpacePos;\n", identifier.AsCharPtr());
				}
				else
				{
					Util::String sTemp = fp.userTypeName + "(1)";
					tmp.Format("templateParam.%s = %s;\n", identifier.AsCharPtr(), sTemp.AsCharPtr());
				}
				code += tmp;
			}
			else if (ShaderUtil::HaveStringAtEnd(identifier, "_ClipSpacePos") )
			{
				tmp.Clear();
				if (m_CustomFunc.vsOutput.bClipSpacePos && type == Forward)
				{
					tmp.Format("templateParam.%s = IN.ClipSpacePos;\n", identifier.AsCharPtr());
				}
				else
				{
					Util::String sTemp = fp.userTypeName + "(1)";
					tmp.Format("templateParam.%s = %s;\n", identifier.AsCharPtr(), sTemp.AsCharPtr());
				}
				code += tmp;
			}
			else if (ShaderUtil::HaveStringAtEnd(identifier, "_ViewDir"))
			{
				tmp.Clear();
				if (m_CustomFunc.vsOutput.bViewDir && type == Forward)
				{
					tmp.Format("templateParam.%s = IN.ViewDir;\n", identifier.AsCharPtr());
				}
				else
				{
					Util::String sTemp = fp.userTypeName + "(1)";
					tmp.Format("templateParam.%s = %s;\n", identifier.AsCharPtr(), sTemp.AsCharPtr());
				}
				code += tmp;
			}
		}

		code += "o.Emission.rgb = float3(0.0f,0.0f,0.0f);";

		if (pSegment->IsEnableVertexLight())
		{
			code += "o.VertexLightDiffuse  = IN.Tangent.xyz;\n";
			code += "o.VertexLightSpecular = IN.Normal.xyz;\n";
		}

		code += pCommon->GetEntryName(CommonShaderSetting::Template) + "(templateParam, o);\n";

	}

	if (pSegment->IsEnalbeAlphaTest())
	{
		code += "  clip (o.Alpha - " + pSegment->GetAlphaTestValue() + ");\n";
		code += "  o.Alpha = 1.0f;\n";
	}
}

void ShaderJoint::_GenerateFinalCode(const CommonShaderSetting* pCommon, const ShadingTemplateSetting* pSegment, const ShaderJoint::PassType& passType, Util::String& code)
{
	Util::String vsCode;

	if (!pCommon->HasEntry(CommonShaderSetting::Vertex))
	{
		vsCode += "struct VS_INPUT\n";
		vsCode += "{\n";
		vsCode += "\tfloat3 vertex : POSITION;\n";
		vsCode += "\tfloat3 normal : NORMAL;\n";
		vsCode += "\tfloat2 texcoord : TEXCOORD0;\n";
		// support lightmap		
		{
			vsCode += "#if LIGHTMAP_ON\n";
			vsCode += "\tfloat2 texcoord1 : TEXCOORD1;\n";
			vsCode += "#endif\n";
		}
		vsCode += "\tfloat4 tangent :  TANGENT;\n";
		vsCode += "\tfloat4 color :  COLOR0;\n";
		vsCode += "};\n";
	}

	code = vsCode + code;

	Util::String vertEntry, pixelEntry;
	if (!pCommon->HasEntry(CommonShaderSetting::Vertex))
	{
		vertEntry = "vsMain";
	}

	pixelEntry = "psMain";

	ShaderJoint::PassInfo info;
	info.vertEntry  = vertEntry;
	info.pixelEntry = pixelEntry;
	info.source     = code;

	m_PassInfos.Add(passType, info);
}

void ShaderJoint::_ReGenRenderState(const ShadingTemplateSetting* pSegment, const Util::String& renderState, const ShaderJoint::PassType& type, Util::String& result)
{
	Util::String passTypeName;
	Util::String passRenderState;
	Util::String matType = "true";

	if (type == ShaderJoint::Depth)
	{
		passTypeName = "DepthPass";
		passRenderState += "cullMode \"Back\"\n";
		passRenderState += "ZTest On\n";
		passRenderState += "ZWrite On\n";
		passRenderState += "blendMode Off\n";
		passRenderState += "alphatest Off\n";
	}
	else if (type == ShaderJoint::Shadow)
	{
		Util::String cullMode;

		IndexT pos = renderState.FindStringIndex("cullMode");

		if (pos == InvalidIndex)
		{
			SYS_EXCEPT(Exceptions::ShaderException, 
				STRING_FORMAT("No cullmode defined in the RenderState!\n.", ""),
				GET_FUNCTION_NAME()
				);
		}

		IndexT nCR = renderState.FindCharIndex('\n', pos);

		cullMode = renderState.ExtractRange(pos, nCR - pos + 1);
		
		passTypeName = "ShadowMap";
		passRenderState += cullMode;
		passRenderState += "\n";

		if (pSegment->IsGenShadow())
		{
			passRenderState += "ZTest On\n";
			passRenderState += "ZWrite On\n";
			passRenderState += "ColorMask \"RGBA\"\n";
		}
		else
		{
			passRenderState += "ZTest Off\n";
			passRenderState += "ZWrite Off\n";
			passRenderState += "ColorMask \"A\"\n";
		}

		passRenderState += "blendMode Off\n";
		passRenderState += "alphatest Off\n";
	}

	result += "}\n";
	result += "MaterialType \"" + matType + "\"\n";
	result += "Pass\t \"" + passTypeName +"\"\t {\n";
	result += "RenderState\n{\n";
	result += passRenderState;
	result += "\t}\n";
}

}