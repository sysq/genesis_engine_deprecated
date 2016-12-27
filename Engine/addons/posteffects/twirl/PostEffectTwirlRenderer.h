#pragma once
/** \file posteffectblurrenderer.h
* \brief post effect component

* Copyright (c) 2011,WebJet Business Division,CYOU
* All rights reserved.
* Date        Ver    Author          Comment
* 2011/12/7   1.0    Qi Song   
*/
#include "graphicsystem/Renderable/QuadRenderer.h"
#include "graphicsystem/base/RenderToTexture.h"

namespace Addons
{
	class PostEffectTwirlRenderer : public Graphic::QuadRenderer
	{
		__DeclareSubClass(PostEffectTwirlRenderer,Graphic::QuadRenderer)

	public:
		PostEffectTwirlRenderer();
		virtual ~PostEffectTwirlRenderer();

		virtual void Setup();
		virtual void RenderObj(const Ptr<Graphic::Renderable>& renderable, bool surface = false,const Ptr<Graphic::Material>& customizedMat = 0);

		float m_twirlRotation;
	};
}