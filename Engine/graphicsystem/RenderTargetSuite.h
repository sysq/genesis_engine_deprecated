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
#ifndef __RenderTargetSuite_H__
#define __RenderTargetSuite_H__

#include "rendersystem/base/RenderDeviceTypes.h"
#include "graphicsystem/base/RenderToTexture.h"

namespace Graphic
{
	class RenderTargetSuite : public Core::RefCounted
	{
		__DeclareClass(RenderTargetSuite);
	public:
		enum Target
		{
			Target_None = BIT_FLAG_NONE,
			Target_RTT = BIT_FLAG(0),
			Target_Swap = BIT_FLAG(1),
			Target_LightLit = BIT_FLAG(2),
			Target_Depth = BIT_FLAG(3),
			Target_All = BIT_FLAG_32BIT_MAX,
		};

		RenderTargetSuite();
		virtual ~RenderTargetSuite();
		void Discard();

		void Setup(int widght, int height, Target target = Target_All, bool delay = true, RenderBase::PixelFormat::Code pixelFormat = RenderBase::PixelFormat::A8R8G8B8, RenderBase::AntiAliasQuality::Code alias = RenderBase::AntiAliasQuality::None);

		void SetupFormRTT(const GPtr<RenderToTexture>& rtt, Target target, bool delay = true);// setup from rendertotexture.

		void OnChangeSize(int widght, int height);

		void OnDeviceReset();

		void ChangeAntiAliasQuality(RenderBase::AntiAliasQuality::Code alias);

		void ChangeTargetSetting(Target target);

		int GetWidth() const;
		int GetHeight() const;
		RenderBase::AntiAliasQuality::Code GetAntiAliasQuality() const;
		RenderTargetSuite::Target GetTargetSetting() const;
		const GPtr<RenderToTexture>& GetRenderToTexture() const;
		const GPtr<RenderToTexture>& GetLightLitTexture() const;
		const GPtr<RenderToTexture>& GetSwapTexture() const;
		const GPtr<RenderToTexture>& GetDepthTexture() const;
	private:

		void setupAll();
		void changeAllSize();
		void setupRenderToTexture() const;
		void setupLightLitTexture() const;
		void setupSwapTexture() const;
		void setupDepthTexture() const;

		mutable GPtr<RenderToTexture> m_renderToTexture;
		mutable GPtr<RenderToTexture> m_swapTexture;

		mutable GPtr<RenderToTexture> m_lightLitTexture;
		mutable GPtr<RenderToTexture> m_depthTexture;

		//// deferred lighting 
		//GPtr<RenderToTexture> m_deferredNormalMap;
		//GPtr<RenderToTexture> m_deferredParamMap;
		//GPtr<RenderToTexture> m_deferredLightMap;

		int m_width;
		int m_height;	
		Target m_targetSetting;
		RenderBase::PixelFormat::Code m_rttFormat;
		RenderBase::AntiAliasQuality::Code m_antiAliasQuality;
	};

	inline int RenderTargetSuite::GetWidth() const
	{
		return m_width;
	}

	inline int RenderTargetSuite::GetHeight() const
	{
		return m_height;
	}

	inline RenderBase::AntiAliasQuality::Code 
		RenderTargetSuite::GetAntiAliasQuality() const
	{
		return m_antiAliasQuality;
	}

	inline RenderTargetSuite::Target 
		RenderTargetSuite::GetTargetSetting() const
	{
		return m_targetSetting;
	}
}
#endif // __RenderTargetSuite_H__
