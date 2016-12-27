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
#include "script_app.h"
#include "graphicsystem/GraphicSystem.h"
#include "resource/resourcemanager.h"
namespace App
{

	static Graphic::ViewPortWindow* gVPW = NULL;


	static Graphic::ViewPortWindow* _getViewportWindow()
	{
		if (gVPW)
		{
			return gVPW;
		}
		return Graphic::GraphicSystem::Instance()->GetMainViewPortWindow();
	}


	CameraComponent* ScriptApp::GetSceneCamera()
	{
		return GetRenderingCamera();
	}

	CameraComponent* ScriptApp::GetRenderingCamera()
	{
		const Graphic::Camera* camera = Graphic::GraphicSystem::Instance()->GetRenderingCamera();
		if (camera)
		{
			Core::RefCounted* owner = camera->GetOwner();
			if (owner)
			{
				CameraComponent* cc = dynamic_cast<CameraComponent *>(owner);
				return cc;
			}
		}
		return NULL;
	}

	void ScriptApp::RenderCamera(Graphic::Camera* camera)
	{
		Graphic::GraphicSystem::Instance()->RenderCamera(camera);
	}

	void ScriptApp::SetRenderTarget(Graphic::RenderToTexture* rt, int index)
	{
		Graphic::GraphicSystem::Instance()->SetRenderTarget(rt, index, RenderBase::RenderTarget::ClearAll);
	}

	int ScriptApp::GetWidth()
	{
		const RenderBase::DisplayMode& dm = _getViewportWindow()->GetDisplayMode();
		return dm.GetWidth();
	}

	int ScriptApp::GetHeight()
	{
		const RenderBase::DisplayMode& dm = _getViewportWindow()->GetDisplayMode();
		return dm.GetHeight();
	}

	GPtr<Graphic::MaterialInstance> ScriptApp::CreateMaterial(const Util::String& matID)
	{
		GPtr<Resources::MaterialResInfo> matResInfo = Resources::ResourceManager::Instance()->CreateMaterialInfo(matID,0);
		if ( !matResInfo.isvalid() )
		{
			return NULL;
		}
		GPtr<Graphic::MaterialInstance>& mtrl = matResInfo->GetHandle();
		//if ( mtrl.isvalid() )
		//{
		//	Util::String defshaderName = "defaultFallBackShader.shader";
		//	int		len = defshaderName.Length();
		//	IndexT pos  = matID.FindStringIndex(defshaderName);
		//	bool bRequestDefaultShader =  (pos != InvalidIndex) && (matID.FindStringIndex(defshaderName,(matID.Length() - len) ) != InvalidIndex );
		//	if ( !bRequestDefaultShader && mtrl->GetName() == Graphic::Material::s_defaultFallBackMat->GetName()  )
		//	{// request create shader is not default shader,and returned material is defaultfallback,then i think it is not exist shader at disk.
		//		return NULL;
		//	}
		//}

		return mtrl;
	}

	void ScriptApp::BlitImage(const RenderBase::TextureHandle* texture, const Graphic::RenderToTexture* target, const Graphic::Material* material, int passIndex)
	{
		Graphic::ImageFiltrationSystem::Render(texture, target, material, passIndex);
	}


	void ScriptApp::_SetTargetWindow(Graphic::ViewPortWindow* vpw)
	{
		gVPW = vpw;
	}
}