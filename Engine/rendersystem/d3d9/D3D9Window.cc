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

#if WIN32



#include "stdneb.h"
#include "RenderDeviceD3D9.h"
#include "D3D9Window.h"


namespace D3D9
{
	D3D9Window::D3D9Window(WindHandle winHandle)
		: RenderBase::RenderWindow( winHandle)
		, m_BeginRender(false)
	{
		//do nothing
	}


	D3D9Window::~D3D9Window()
	{	
		//do nothing
	}

	void D3D9Window::BeginRender()
	{
		//m_RenderDevice->BeginFrame();
		m_BeginRender = true;
	}

	void D3D9Window::EndRender()
	{
		//m_RenderDevice->EndFrame();
		//m_RenderDevice->Present(m_WinHandle);
		if (m_BeginRender)
		{
			RenderDeviceD3D9::Instance()->SetSwapChain(m_TargetHandle);
			RenderDeviceD3D9::Instance()->Present(m_WinHandle);
			m_BeginRender = false;
		}
	}
}

#endif