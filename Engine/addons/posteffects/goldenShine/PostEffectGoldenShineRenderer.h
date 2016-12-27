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
	class PostEffectGoldenShineRenderer : public Graphic::QuadRenderer
	{
		__DeclareSubClass(PostEffectGoldenShineRenderer,Graphic::QuadRenderer)

	public:
		PostEffectGoldenShineRenderer();
		virtual ~PostEffectGoldenShineRenderer();

		virtual void Setup();
		virtual void RenderObj(const Ptr<Graphic::Renderable>& renderable, Graphic::SurfacePassType surface = Graphic::eCustomized,const Ptr<Graphic::Material>& customizedMat = 0);

		bool m_bRTBinded;
	};
}