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
#include "graphicsystem/Camera/RenderPipeline/RenderData.h"
#include "guifeature/guiserver.h"
#include "guifeature/guirenderobject.h"

namespace App
{


	__ImplementClass(GuiRenderObject,'GUOB',Graphic::RenderObject);

	GuiRenderObject::GuiRenderObject()
		:m_renderable(NULL)
	{
		SetLayerID(0);
	}

	GuiRenderObject::~GuiRenderObject()
	{
		n_delete(m_renderable);
		m_renderable = NULL;
	}

	void GuiRenderObject::Setup()
	{
		if (m_renderable == NULL)
		{
			m_renderable = n_new(GuiRenderable);
			m_renderable->SetMaterial(GUIServer::Instance()->GetGuiMaterial());
		}
	}

	void GuiRenderObject::AddToCollection(Graphic::RenderDataCollection* collection, const Graphic::RenderConfig* renderConfig)
	{
		if (m_renderable)
		{
			collection->Push(m_renderable);
		}
	}

	void GuiRenderObject::Render(const Graphic::Renderable* renderable, Graphic::RenderPassType passType, const Graphic::Material* customizedMaterial)
	{
		GUIServer::Instance()->_renderUI();
	}

	void GuiRenderObject::OnWillRenderObject(Graphic::Camera* sender)
	{

	}

}
