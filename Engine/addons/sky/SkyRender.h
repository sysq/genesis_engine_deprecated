#pragma once

#include "graphicsystem/Renderable/GraphicRenderer.h"


namespace Sky
{

class SkyRenderer : public Graphic::GraphicRenderer
{
	__DeclareSubClass(SkyRenderer, GraphicRenderer);

public:
	
	SkyRenderer();

	~SkyRenderer();

	virtual void RenderObj(const GPtr<Graphic::Renderable>& renderable, Graphic::SurfacePassType surface = Graphic::eCustomized, const GPtr<Graphic::Material>& customizedMat = 0);
};

class MoonRenderer : public Graphic::GraphicRenderer
{
	__DeclareSubClass(MoonRenderer, GraphicRenderer);

public:

	MoonRenderer();

	~MoonRenderer();

	virtual void RenderObj(const GPtr<Graphic::Renderable>& renderable, Graphic::SurfacePassType surface = Graphic::eCustomized, const GPtr<Graphic::Material>& customizedMat = 0);

};

}