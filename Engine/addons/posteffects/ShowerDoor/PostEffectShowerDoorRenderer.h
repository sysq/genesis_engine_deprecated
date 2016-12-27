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
	class PostEffectShowerDoorRenderer : public Graphic::QuadRenderer
	{
		__DeclareSubClass(PostEffectShowerDoorRenderer,Graphic::QuadRenderer)

	public:
		PostEffectShowerDoorRenderer();
		virtual ~PostEffectShowerDoorRenderer();

		virtual void Setup();
		virtual void RenderObj(const Ptr<Graphic::Renderable>& renderable, Graphic::SurfacePassType surface = Graphic::eCustomized,const Ptr<Graphic::Material>& customizedMat = 0);

		float m_bumpAmount;
		RenderBase::TextureHandle m_bumpMapHandle;
		bool m_bRTBinded;
	};
}