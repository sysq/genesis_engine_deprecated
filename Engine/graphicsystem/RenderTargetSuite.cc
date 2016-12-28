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
#include "RenderTargetSuite.h"
namespace Graphic
{
	__ImplementClass(RenderTargetSuite,'RDTS',Core::RefCounted);
	RenderTargetSuite::RenderTargetSuite()
		:m_width(0)
		,m_height(0)
		,m_antiAliasQuality(RenderBase::AntiAliasQuality::None)
		,m_rttFormat(RenderBase::PixelFormat::InvalidPixelFormat)
	{
	}

	RenderTargetSuite::~RenderTargetSuite()
	{
		Discard();
	}

	void RenderTargetSuite::Setup(int widght, int height, Target target /* = Target_All */, bool delay /* = true */, RenderBase::PixelFormat::Code pixelFormat /* = RenderBase::PixelFormat::A8R8G8B8 */, RenderBase::AntiAliasQuality::Code alias /* = RenderBase::AntiAliasQuality::None */)
	{
		m_width = widght;
		m_height = height;
		m_antiAliasQuality = alias;
		m_rttFormat = pixelFormat;
		m_targetSetting = target;

		if (delay)
		{
			if (BIT_FLAG_IS_OPEN(m_targetSetting, Target_RTT))
			{
				m_renderToTexture = RenderToTexture::Create();
			}
			setupRenderToTexture();
		}
		else
		{

			if (BIT_FLAG_IS_OPEN(m_targetSetting, Target_Swap))
			{
				m_swapTexture = RenderToTexture::Create();
			}
			if (BIT_FLAG_IS_OPEN(m_targetSetting, Target_Depth))
			{
				m_depthTexture = RenderToTexture::Create();
			}
			if (BIT_FLAG_IS_OPEN(m_targetSetting, Target_LightLit))
			{
				m_lightLitTexture = RenderToTexture::Create();
			}
			setupAll();
		}
	}

	void RenderTargetSuite::SetupFormRTT(const GPtr<RenderToTexture>& rtt, Target target, bool delay /* = true */)
	{
		m_width = rtt->GetWidth();
		m_height = rtt->GetHeight();
		m_antiAliasQuality = rtt->GetAntiAliasQuality();
		m_rttFormat = rtt->GetColorBufferFormat();
		m_targetSetting = BIT_FlAG_APPEND(target, Target_RTT);
		m_renderToTexture = rtt;

		if (!delay)
		{
			if (BIT_FLAG_IS_OPEN(m_targetSetting, Target_Swap))
			{
				m_swapTexture = RenderToTexture::Create();
				setupSwapTexture();
			}
			if (BIT_FLAG_IS_OPEN(m_targetSetting, Target_Depth))
			{
				m_depthTexture = RenderToTexture::Create();
				setupDepthTexture();
			}
			if (BIT_FLAG_IS_OPEN(m_targetSetting, Target_LightLit))
			{
				m_lightLitTexture = RenderToTexture::Create();
				setupLightLitTexture();
			}
		}
	}

	void RenderTargetSuite::Discard()
	{
		m_renderToTexture = NULL;
		m_swapTexture = NULL;
		m_lightLitTexture = NULL;
		m_depthTexture = NULL;
	}

	void RenderTargetSuite::OnChangeSize(int widght, int height)
	{
		if (m_width != widght || m_height != height)
		{
			m_width = widght;
			m_height = height;
			changeAllSize();
		}
	}

	void RenderTargetSuite::ChangeAntiAliasQuality(RenderBase::AntiAliasQuality::Code alias)
	{
		m_antiAliasQuality = alias;
		if (m_renderToTexture.isvalid())
		{
			setupRenderToTexture();
		}
	}

	void RenderTargetSuite::ChangeTargetSetting(Target target)
	{
		m_targetSetting = target;
		if (!BIT_FLAG_IS_OPEN(m_targetSetting, Target_Swap))
		{
			if (m_swapTexture.isvalid())
			{
				m_swapTexture = NULL;
			}
		}
		if (!BIT_FLAG_IS_OPEN(m_targetSetting, Target_Depth))
		{
			if (m_depthTexture.isvalid())
			{
				m_depthTexture = NULL;
			}
		}
		if (!BIT_FLAG_IS_OPEN(m_targetSetting, Target_LightLit))
		{
			if (m_lightLitTexture.isvalid())
			{
				m_lightLitTexture = NULL;
			}
		}		
		if (BIT_FLAG_IS_OPEN(m_targetSetting, Target_RTT))
		{
			if (!m_renderToTexture.isvalid())
			{
				m_renderToTexture = RenderToTexture::Create();
				setupRenderToTexture();
			}
		}
		else
		{
			if (m_renderToTexture.isvalid())
			{
				m_renderToTexture = NULL;
			}
		}
	}

	void RenderTargetSuite::OnDeviceReset()
	{
		if (m_renderToTexture)
		{
			m_renderToTexture->ChangeSize(m_width, m_height);
		}

		if (m_swapTexture)
		{
			m_renderToTexture->ChangeSize(m_width, m_height);
		}

		if (m_depthTexture)
		{
			m_renderToTexture->ChangeSize(m_width, m_height);
		}

		if (m_lightLitTexture)
		{
			m_renderToTexture->ChangeSize(m_width, m_height);
		}
	}



	const GPtr<RenderToTexture>& RenderTargetSuite::GetRenderToTexture() const
	{
		if (BIT_FLAG_IS_OPEN(m_targetSetting, Target_RTT) && !m_renderToTexture.isvalid())
		{
			m_renderToTexture = RenderToTexture::Create();
			setupRenderToTexture();
		}
		return m_renderToTexture;
	}

	const GPtr<RenderToTexture>& RenderTargetSuite::GetLightLitTexture() const
	{
		if (BIT_FLAG_IS_OPEN(m_targetSetting, Target_LightLit) && !m_lightLitTexture.isvalid())
		{
			m_lightLitTexture = RenderToTexture::Create();
			setupLightLitTexture();
		}
		return m_lightLitTexture;
	}

	const GPtr<RenderToTexture>& RenderTargetSuite::GetSwapTexture() const
	{
		if (BIT_FLAG_IS_OPEN(m_targetSetting, Target_Swap) && !m_swapTexture.isvalid())
		{
			m_swapTexture = RenderToTexture::Create();
			setupSwapTexture();
		}
		return m_swapTexture;
	}

	const GPtr<RenderToTexture>& RenderTargetSuite::GetDepthTexture() const
	{
		if (BIT_FLAG_IS_OPEN(m_targetSetting, Target_Depth) && !m_depthTexture.isvalid())
		{
			m_depthTexture = RenderToTexture::Create();
			setupDepthTexture();
		}
		return m_depthTexture;
	}

	void RenderTargetSuite::setupAll()
	{
		setupRenderToTexture();
		setupSwapTexture();
		setupLightLitTexture();
		setupDepthTexture();
	}

	inline bool _needUpdateSize(const GPtr<RenderToTexture>& rtt, int w, int h)
	{
		if (rtt.isvalid())
		{
			return rtt->GetWidth() != w || rtt->GetHeight() != h;
		}
		return false;
	}

	void RenderTargetSuite::changeAllSize()
	{
		if (_needUpdateSize(m_renderToTexture, m_width, m_height))
		{
			setupRenderToTexture();
		}
		if (_needUpdateSize(m_swapTexture, m_width, m_height))
		{
			setupSwapTexture();
		}
		if (_needUpdateSize(m_lightLitTexture, m_width, m_height))
		{
			setupLightLitTexture();
		}
		if (_needUpdateSize(m_depthTexture, m_width, m_height))
		{
			setupDepthTexture();
		}
	}

	void RenderTargetSuite::setupRenderToTexture() const
	{
		if (BIT_FLAG_IS_OPEN(m_targetSetting, Target_RTT))
		{
			if (m_renderToTexture.isvalid())
			{
				m_renderToTexture->Setup(m_width, m_height, m_rttFormat, 
					RenderBase::RenderTarget::ClearAll, Math::float4(0.2f,0.2f,0.2f,1.f), true, 1.f, m_antiAliasQuality);
			}
		}
	}

	void RenderTargetSuite::setupSwapTexture() const
	{
		if (BIT_FLAG_IS_OPEN(m_targetSetting, Target_Swap))
		{
			if (m_swapTexture.isvalid())
			{
				m_swapTexture->Setup(m_width, m_height, m_rttFormat, RenderBase::RenderTarget::ClearAll, 
					Math::float4(0.f, 0.f, 0.f, 1.f), false, 1.f, RenderBase::AntiAliasQuality::None);
			}
		}
	}

	void RenderTargetSuite::setupDepthTexture() const
	{
		if (BIT_FLAG_IS_OPEN(m_targetSetting, Target_Depth))
		{
			if (m_depthTexture.isvalid())
			{
				m_depthTexture = NULL;
				m_depthTexture = RenderToTexture::Create();
				if (m_renderToTexture)
				{
					m_depthTexture->SetShareDepthStencilRT(m_renderToTexture->GetRenderTarget());
				}
#if (__WIN32__ || __WIN64__) && RENDERDEVICE_D3D9
				RenderBase::PixelFormat::Code format = RenderBase::PixelFormat::R32F;
#elif __ANDROID__	|| RENDERDEVICE_OPENGLES
				RenderBase::PixelFormat::Code format = RenderBase::PixelFormat::A8R8G8B8;
#endif
				m_depthTexture->Setup(m_width, m_height, format, 
					RenderBase::RenderTarget::ClearAll, Math::float4(1.f,1.f,1.f,1.f), 
					true,1.0f, RenderBase::AntiAliasQuality::None);
			}
		}
	}

	void RenderTargetSuite::setupLightLitTexture() const
	{
		if (BIT_FLAG_IS_OPEN(m_targetSetting, Target_LightLit))
		{
			if (m_lightLitTexture.isvalid())
			{
				m_lightLitTexture->Setup(m_width, m_height, RenderBase::PixelFormat::R16F, 
					RenderBase::RenderTarget::ClearAll, Math::float4(0.f,0.f,0.f,1.f), 
					true,1.f);
			}
		}
	}
}
