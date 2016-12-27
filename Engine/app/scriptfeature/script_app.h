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
#ifndef __scriptapp_H__
#define __scriptapp_H__

#include "graphicsystem/Material/materialinstance.h"
#include "graphicsystem/Camera/Camera.h"
#include "graphicsystem/base/RenderToTexture.h"
#include "graphicfeature/components/cameracomponent.h"
namespace App
{
	class ScriptApp
	{
	public:
		static CameraComponent* GetSceneCamera();
		static CameraComponent* GetRenderingCamera();
		static void RenderCamera(Graphic::Camera* camera);
		static void SetRenderTarget(Graphic::RenderToTexture* rt, int index);
		static int GetWidth();
		static int GetHeight();
		static GPtr<Graphic::MaterialInstance> CreateMaterial(const Util::String& matID); 
		static void BlitImage(const RenderBase::TextureHandle* texture, const Graphic::RenderToTexture* target, const Graphic::Material* material, int passIndex);
		

		static void _SetTargetWindow(Graphic::ViewPortWindow* vpw);
	};
}



#endif //__scriptapp_H__