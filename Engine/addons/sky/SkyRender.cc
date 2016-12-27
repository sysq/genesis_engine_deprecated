#include "stdneb.h"
#include "SkySphere.h"
#include "Moon.h"
#include "SkyRender.h"
#include "graphicsystem/GraphicSystem.h"
#include "graphicsystem/Material/materialinstance.h"
#include "resource/resourcemanager.h"

namespace Sky
{
	__ImplementClass(SkyRenderer, 'SRCP', Graphic::GraphicRenderer);
	__ImplementClass(MoonRenderer, 'MORD', Graphic::GraphicRenderer);

	using namespace  Graphic;

SkyRenderer::SkyRenderer()
{

}

SkyRenderer::~SkyRenderer()
{

}

void SkyRenderer::RenderObj(const GPtr<Graphic::Renderable>& renderable, SurfacePassType surface, const GPtr<Graphic::Material>& customizedMat)
{
	//[zhongdaohuan][render_obj]临时删掉实现

	
}

MoonRenderer::MoonRenderer()
{

}

MoonRenderer::~MoonRenderer()
{

}

void MoonRenderer::RenderObj(const GPtr<Graphic::Renderable>& renderable, SurfacePassType surface, const GPtr<Graphic::Material>& customizedMat)
{
	//[zhongdaohuan][render_obj]临时删掉实现


}

}

