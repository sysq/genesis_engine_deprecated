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
#include "basegamefeature/managers/sceneschedulemanager.h"
#include "guifeature/guiserver.h"
#include "guifeature/guiscene.h"
#include "guifeature/guirenderobject.h"
namespace App
{


	GuiScene::GuiScene()
	{

	}

	GuiScene::~GuiScene()
	{

	}

	void GuiScene::Setup()
	{
		m_scene = SceneScheduleManager::Instance()->OpenScene("gui scene for com.genesis", true, true);
		m_scene->GetRenderScene()->SetRenderSort(Graphic::RenderScene::SortUI);

		m_guiCamera = SimpleCamera::Create();
		m_guiCamera->SetRenderSort(Graphic::Camera::SortUI);
		m_guiCamera->SetPickMark(Graphic::Camera::PickSelfObjects);
		m_guiCamera->SetUseCallBack(false);
		m_guiCamera->Setup();
		m_guiCamera->SetProj(Graphic::Camera::VT_ortho, 1.0f, -10.0f, 10.0f);
		m_guiCamera->SetTransform(Math::matrix44::identity());
		m_guiCamera->OnTransformChanged();

		m_guiCamera->SetTargetWindow(GUIServer::Instance()->GetTargetWindow());

		m_scene->GetRenderScene()->_AddCamera(m_guiCamera.get_unsafe());

		m_guiObject = GuiRenderObject::Create();
		m_guiObject->Setup();
		m_guiObject->AttachNoCull(m_guiCamera->GetCameraScene());
	}



	void GuiScene::Destroy()
	{
		m_guiObject->Detach();
		m_guiCamera->GetRenderScene()->_RemoveCamera(m_guiCamera.get_unsafe());
		SceneScheduleManager::Instance()->CloseScene(m_scene);
		m_scene = NULL;
	}

	void GuiScene::SetTargetWindow(Graphic::ViewPortWindow* vpw)
	{
		if (m_guiCamera.isvalid())
		{
			m_guiCamera->SetTargetWindow(vpw);
		}
	}

}
