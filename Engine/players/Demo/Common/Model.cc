/** \file: DrawModel.cc
* \brief: Draw model
* Copyright (c) 2011,WebJet Enterprise Department.ChangYou
* All rights reserved.
* Date        Ver    Who          Comment
* 2011/10/25   1.0    SunHao   
*/

#include "stdneb.h"
#include "Model.h"
#include "appframework/actor.h"
#include "math/polar.h"
#include "DemoGameStateHandler.h"
#include "basegamefeature/basegamefeature.h"
#include "basegamefeature/managers/sceneschedulemanager.h"
#include "graphicfeature/graphicsfeature.h"
#include "appframework/gameserver.h"
#include "appframework/scene.h"
#include "appframework/actor.h"
#include "graphicfeature/components/cameracomponent.h"
#include "graphicfeature/components/lightcomponent.h"
#include "graphicfeature/components/meshrendercomponent.h"
#include "input/inputserver.h"
#include "graphicsystem/GraphicSystem.h"
#include "graphicsystem/Light/Light.h"
#include "graphicsystem/Camera/Camera.h"
#include "addons/resource/meshres.h"
#include "apputil/manuresutil.h"
#include "graphicfeature/graphicsfeature.h"
#include "graphicfeature/graphicsfeatureprotocol.h"
#include "resource/resourceserver.h"
#include "input/inputmouse.h"
#include "math/ray.h"
#include "apputil/intersectutil.h"
#include "app/appframework/app_fwd_decl.h"
#include "input/input_fwd_decl.h"
#include "foundation/util/array.h"

namespace Demo 
{

	App::TActorPtr DrawABunker( const Util::String& sActorName/*="Bunker"*/ )
	{
		using namespace App;

		TActorPtr pActor = Actor::Create();
		pActor->SetName( sActorName );

		Ptr<MeshRenderComponent> pMeshRenderComponent = MeshRenderComponent::Create();
		pMeshRenderComponent->SetMeshID( "msh:Bunker.mesh" );
		pMeshRenderComponent->SetMaterialByShaderID(0, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(0, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(0, "_diffuseMap", "tex:Bunker.jpg" );

		pActor->AttachComponent( pMeshRenderComponent.upcast<Component>() );

		return pActor;
	}

	App::TActorPtr DrawATree( const Util::String& sActorName/*="Tree"*/ )
	{
		using namespace App;

		TActorPtr pActor = Actor::Create();
		pActor->SetName( sActorName );


		Ptr<MeshRenderComponent> pMeshRenderComponent = MeshRenderComponent::Create();
		pMeshRenderComponent->SetMeshID( "msh:Tree.mesh" );
		pMeshRenderComponent->SetMaterialByShaderID(0, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(0, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(0, "_diffuseMap", "tex:Bunker.jpg" );


		pMeshRenderComponent->SetMaterialByShaderID(1, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(1, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(1, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(1, "_diffuseMap", "tex:Bunker.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(2, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(2, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(2, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(2, "_diffuseMap", "tex:wood_sheets_moosy_diff.dds" );

		pActor->AttachComponent( pMeshRenderComponent.upcast<Component>() );

		Math::quaternion rot(1.2f, 1.f, 0.f, 0.f );
		pActor->SetRotation(rot);
		return pActor;
	}

	App::TActorPtr DrawABmw7( const Util::String& sActorName/*="BMW7Series"*/ )
	{
		using namespace App;

		App::TActorPtr pActor = Actor::Create();
		pActor->SetName( sActorName );

		

		Ptr<MeshRenderComponent> pMeshRenderComponent = MeshRenderComponent::Create();
		pMeshRenderComponent->SetMeshID( "msh:bmw7.mesh" );
		pMeshRenderComponent->SetMaterialByShaderID(0, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(0, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(0, "_diffuseMap", "tex:Bunker.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(1, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(1, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(1, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(1, "_diffuseMap", "tex:Bunker.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(2, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(2, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(2, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(2, "_diffuseMap", "tex:wood_sheets_moosy_diff.dds" );

		pMeshRenderComponent->SetMaterialByShaderID(3, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(3, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(3, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(3, "_diffuseMap", "tex:Bunker.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(4, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(4, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(4, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(4, "_diffuseMap", "tex:wood_sheets_moosy_diff.dds" );

		pMeshRenderComponent->SetMaterialByShaderID(5, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(5, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(5, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(5, "_diffuseMap", "tex:Bunker.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(6, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(6, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(6, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(6, "_diffuseMap", "tex:wood_sheets_moosy_diff.dds" );

		pMeshRenderComponent->SetMaterialByShaderID(7, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(7, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(7, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(7, "_diffuseMap", "tex:Bunker.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(8, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(8, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(8, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(8, "_diffuseMap", "tex:wood_sheets_moosy_diff.dds" );

		pMeshRenderComponent->SetMaterialByShaderID(9, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(9, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(9, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(9, "_diffuseMap", "tex:Bunker.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(10, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(10, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(10, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(10, "_diffuseMap", "tex:Bunker.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(11, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(11, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(11, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(11, "_diffuseMap", "tex:wood_sheets_moosy_diff.dds" );

		pMeshRenderComponent->SetMaterialByShaderID(12, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(12, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(12, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(12, "_diffuseMap", "tex:Bunker.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(13, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(13, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(13, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(13, "_diffuseMap", "tex:Bunker.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(14, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(14, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(14, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(14, "_diffuseMap", "tex:Bunker.jpg" );

		pActor->AttachComponent( pMeshRenderComponent.upcast<Component>() );

		Math::quaternion rot(0.f, 1.f, 0.f, 0.f );
		pActor->SetRotation(rot);

		return pActor;

	}

	App::TActorPtr DrawACube( const Util::String& sAcotrName/*="Cube"*/ )
	{
		using namespace App;
		using namespace Resources;

		// 构建一个mesh来进行测试
		ResourceServer* resServer = ResourceServer::Instance();
		Ptr<Resource> pRes = resServer->CreateOrGetResource( "emptyResource.mesh", &MeshRes::RTTI );
		Ptr<MeshRes> pMeshRes = pRes.downcast<MeshRes>();

		// 构建一个正方形
		SizeT NumVertex = 8;

		float hWidth = float(1 * 0.5);
		float hHeight = float(1 * 0.5);
		float hDepth = float(1 * 0.5);

		bool bOK = true;

		// 位置
		Util::Array<PositionData::Elem> vertices;
		{
			vertices.Resize( NumVertex , PositionData::Elem() );

			//0
			vertices[0].x() = -hWidth;
			vertices[0].y() = -hHeight;
			vertices[0].z() = -hDepth;
			//1
			vertices[1].x() = hWidth;
			vertices[1].y() = -hHeight;
			vertices[1].z() = -hDepth;
			//2
			vertices[2].x() = hWidth;
			vertices[2].y() = hHeight;
			vertices[2].z() = -hDepth;
			//3
			vertices[3].x() = -hWidth;
			vertices[3].y() = hHeight;
			vertices[3].z() = -hDepth;
			//4
			vertices[4].x() = -hWidth;
			vertices[4].y() = -hHeight;
			vertices[4].z() = hDepth;
			//5
			vertices[5].x() = hWidth;
			vertices[5].y() = -hHeight;
			vertices[5].z() = hDepth;
			//6
			vertices[6].x() = hWidth;
			vertices[6].y() = hHeight;
			vertices[6].z() = hDepth;
			//7
			vertices[7].x() = -hWidth;
			vertices[7].y() = hHeight;
			vertices[7].z() = hDepth;
		}
		bOK = pMeshRes->SetVertexData<PositionData>( &vertices[0], NumVertex );
		n_assert(bOK);

		// 颜色
		Util::Array<ColorData::Elem> colors;
		{
			colors.Resize(NumVertex, ColorData::Elem() );

			colors[0].Set(0, 255, 0, 0);
			colors[1].Set(0, 255, 0, 0);
			colors[2].Set(0, 255, 0, 0);
			colors[3].Set(0, 255, 0, 0);
			colors[4].Set(0, 255, 0, 0);
			colors[5].Set(0, 255, 0, 0);
			colors[6].Set(0, 255, 0, 0);
			colors[7].Set(0, 255, 0, 0);
		}
		bOK = pMeshRes->SetVertexData<ColorData>( &colors[0], NumVertex );
		n_assert(bOK);

		// 纹理
		Util::Array<TexCoordData::Elem> texCoords;
		{
			texCoords.Resize(NumVertex, TexCoordData::Elem() );

			//0
			texCoords[0].x() = 0;
			texCoords[0].y() = 0;
			//1
			texCoords[1].x() = 0;
			texCoords[1].y() = 1;
			//2
			texCoords[2].x() = 1;
			texCoords[2].y() = 1;
			//3
			texCoords[3].x() = 1;
			texCoords[3].y() = 0;
			//4
			texCoords[4].x() = 0;
			texCoords[4].y() = 1;
			//5
			texCoords[5].x() = 0;
			texCoords[5].y() = 0;
			//6
			texCoords[6].x() = 1;
			texCoords[6].y() = 0;
			//7
			texCoords[7].x() = 1;
			texCoords[7].y() = 1;
		}
		bOK = pMeshRes->SetVertexData<TexCoordData>( &texCoords[0], NumVertex );
		n_assert(bOK);


		// 索引
		ushort indices[] =
		{
			0,4,5,
			0,5,1,
			1,5,6,
			1,6,2,
			2,6,7,
			2,7,3,
			3,7,4,
			3,4,0,				
			4,7,6,
			4,6,5,				
			3,0,1,
			3,1,2,
		};

		bOK = pMeshRes->SetIndex16(indices, 36 );
		n_assert(bOK);

		SubMesh subMesh;
		subMesh.firstVertex = 0;
		subMesh.numVertex = NumVertex;
		subMesh.FirstIndex = 0;
		subMesh.numIndex = 36;
		bOK = pMeshRes->SetSubMesh(&subMesh, 0);

		pMeshRes->SetTopologyType( RenderBase::PrimitiveTopology::TriangleList );
		pMeshRes->SetState( Resource::Loaded , MaxResClipCount );
		pMeshRes->RecalculateBounds(true);
		TActorPtr pActor = App::Actor::Create();


		Ptr<App::MeshRenderComponent> pMeshRenderComponent = App::MeshRenderComponent::Create();
		pMeshRenderComponent->SetMeshID(pMeshRes->GetResourceId());
		pMeshRenderComponent->SetMaterialByShaderID(0, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(0, "_diffuseMap", "sys:wood_sheets_moosy_diff.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(0, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(0, "_diffuseMap", "tex:wood_sheets_moosy_diff.dds" );

		pActor->AttachComponent( pMeshRenderComponent.upcast<App::Component>() );

		return pActor;
	}

	App::TActorPtr DrawAVarietyShop( const Util::String& sActorName/*="VarietyShop"*/ )
	{
		using namespace App;

		App::TActorPtr pActor = Actor::Create();
		pActor->SetName( sActorName );
		

		Ptr<MeshRenderComponent> pMeshRenderComponent = MeshRenderComponent::Create();
		pMeshRenderComponent->SetMeshID( "msh:VarietyShop.mesh" );
		pMeshRenderComponent->SetMaterialByShaderID(0, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(0, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(0, "_diffuseMap", "tex:Bunker.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(1, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(1, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(1, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(1, "_diffuseMap", "tex:Bunker.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(2, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(2, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(2, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(2, "_diffuseMap", "tex:wood_sheets_moosy_diff.dds" );

		pMeshRenderComponent->SetMaterialByShaderID(3, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(3, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(3, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(3, "_diffuseMap", "tex:Bunker.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(4, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(4, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(4, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(4, "_diffuseMap", "tex:wood_sheets_moosy_diff.dds" );

		pMeshRenderComponent->SetMaterialByShaderID(5, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(5, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(5, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(5, "_diffuseMap", "tex:Bunker.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(6, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(6, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(6, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(6, "_diffuseMap", "tex:wood_sheets_moosy_diff.dds" );

		pMeshRenderComponent->SetMaterialByShaderID(7, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(7, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(7, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(7, "_diffuseMap", "tex:Bunker.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(8, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(8, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(8, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(8, "_diffuseMap", "tex:wood_sheets_moosy_diff.dds" );

		pMeshRenderComponent->SetMaterialByShaderID(9, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(9, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(9, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(9, "_diffuseMap", "tex:Bunker.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(10, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(10, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(10, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(10, "_diffuseMap", "tex:Bunker.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(11, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(11, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(11, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(11, "_diffuseMap", "tex:wood_sheets_moosy_diff.dds" );

		pMeshRenderComponent->SetMaterialByShaderID(12, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(12, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(12, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(12, "_diffuseMap", "tex:Bunker.jpg" );

		pActor->AttachComponent( pMeshRenderComponent.upcast<Component>() );

		return pActor;		
	}

	App::TActorPtr DrawHouse1( const Util::String& sActorName/*="House1"*/ )
	{
		using namespace App;

		App::TActorPtr pActor = Actor::Create();
		pActor->SetName( sActorName );
		

		Ptr<MeshRenderComponent> pMeshRenderComponent = MeshRenderComponent::Create();
		pMeshRenderComponent->SetMeshID( "msh:House_01.mesh" );
		pMeshRenderComponent->SetMaterialByShaderID(0, "shd:Forward_SimpleWithShadow.wjshader");
		pMeshRenderComponent->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(0, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(0, "_diffuseMap", "tex:House_01_zhu.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(1, "shd:Forward_SimpleWithShadow.wjshader");
		pMeshRenderComponent->_AddTextureParam(1, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(1, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(1, "_diffuseMap", "tex:House_01_qiang.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(2, "shd:Forward_SimpleWithShadow.wjshader");
		pMeshRenderComponent->_AddTextureParam(2, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(2, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(2, "_diffuseMap", "tex:House_01_wa.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(3, "shd:Forward_SimpleWithShadow.wjshader");
		pMeshRenderComponent->_AddTextureParam(3, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(3, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(3, "_diffuseMap", "tex:House_01_chuang.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(4, "shd:Forward_SimpleWithShadow.wjshader");
		pMeshRenderComponent->_AddTextureParam(4, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(4, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(4, "_diffuseMap", "tex:House_01_05.jpg" );

		pActor->AttachComponent( pMeshRenderComponent.upcast<Component>() );

		return pActor;		
	}

	App::TActorPtr DrawHouse2( const Util::String& sActorName/*="House2"*/ )
	{
		using namespace App;

		App::TActorPtr pActor = Actor::Create();
		pActor->SetName( sActorName );
		

		Ptr<MeshRenderComponent> pMeshRenderComponent = MeshRenderComponent::Create();
		pMeshRenderComponent->SetMeshID( "msh:House_02.mesh" );
		// Wa
		pMeshRenderComponent->SetMaterialByShaderID(0, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(0, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(0, "_diffuseMap", "tex:House_02_Wa.jpg" );

		// Qiang2
		pMeshRenderComponent->SetMaterialByShaderID(1, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(1, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(1, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(1, "_diffuseMap", "tex:House_02_Qiang2.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(2, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(2, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(2, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(2, "_diffuseMap", "tex:House_02_Qiang1.jpg" );

		// lian zi
		pMeshRenderComponent->SetMaterialByShaderID(3, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(3, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(3, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(3, "_diffuseMap", "tex:House_02_LianZi.jpg" );

		// gua yu
		pMeshRenderComponent->SetMaterialByShaderID(4, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(4, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(4, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(4, "_diffuseMap", "tex:House_02_GuaYu.tga" );

		pMeshRenderComponent->SetMaterialByShaderID(5, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(5, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(5, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(5, "_diffuseMap", "tex:House_02_Qiang1.jpg" );

		pActor->AttachComponent( pMeshRenderComponent.upcast<Component>() );

		return pActor;		
	}

	App::TActorPtr DrawTree01( const Util::String& sActorName/*="Tree_01"*/ )
	{
		using namespace App;

		App::TActorPtr pActor = Actor::Create();
		pActor->SetName( sActorName );
		

		Ptr<MeshRenderComponent> pMeshRenderComponent = MeshRenderComponent::Create();
		pMeshRenderComponent->SetMeshID( "msh:Tree_01.mesh" );
		pMeshRenderComponent->SetMaterialByShaderID(0, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(0, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(0, "_diffuseMap", "tex:Tree_01_01.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(1, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(1, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(1, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(1, "_diffuseMap", "tex:Tree_01_02.tga" );

		pActor->AttachComponent( pMeshRenderComponent.upcast<Component>() );

		return pActor;		
	}

	App::TActorPtr DrawMengGuBao01( const Util::String& sActorName/*="MengGuBao01"*/ )
	{
		using namespace App;

		App::TActorPtr pActor = Actor::Create();
		pActor->SetName( sActorName );
		

		Ptr<MeshRenderComponent> pMeshRenderComponent = MeshRenderComponent::Create();
		pMeshRenderComponent->SetMeshID( "msh:MengGuBao_01.mesh" );
		pMeshRenderComponent->SetMaterialByShaderID(0, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(0, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(0, "_diffuseMap", "tex:MengGuBao_01.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(1, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(1, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(1, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(1, "_diffuseMap", "tex:MengGuBao_02.jpg" );

		pActor->AttachComponent( pMeshRenderComponent.upcast<Component>() );

		return pActor;		
	}

	App::TActorPtr DrawStreetLight01( const Util::String& sActorName/*="StreetLight01"*/ )
	{
		using namespace App;

		App::TActorPtr pActor = Actor::Create();
		pActor->SetName( sActorName );

		

		Ptr<MeshRenderComponent> pMeshRenderComponent = MeshRenderComponent::Create();
		pMeshRenderComponent->SetMeshID( "msh:StreetLight_01.mesh" );
		pMeshRenderComponent->SetMaterialByShaderID(0, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(0, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(0, "_diffuseMap", "tex:StreetLight01_DiZuo.jpg" );

		// Zhu Zi
		pMeshRenderComponent->SetMaterialByShaderID(1, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(1, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(1, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(1, "_diffuseMap", "tex:StreetLight01_ZhuZi.jpg" );

		// Deng Long
		pMeshRenderComponent->SetMaterialByShaderID(2, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(2, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(2, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(2, "_diffuseMap", "tex:StreetLight01_DengLong.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(3, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(3, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(3, "_diffuseMap") );
		//pMeshRenderComponent->SetTexture(3, "_diffuseMap", "tex:StreetLight01_WaLiang.jpg" );

		pActor->AttachComponent( pMeshRenderComponent.upcast<Component>() );

		return pActor;				
	}

	App::TActorPtr DrawCJ_chitang001( const Util::String& sActorName/*="House1"*/ )
	{
		using namespace App;

		App::TActorPtr pActor = Actor::Create();
		pActor->SetName( sActorName );
		

		Ptr<MeshRenderComponent> pMeshRenderComponent = MeshRenderComponent::Create();
		pMeshRenderComponent->SetMeshID( "msh:cj_chitang001.mesh" );
		pMeshRenderComponent->SetMaterialByShaderID(0, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(0, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(0, "_diffuseMap", "tex:heye001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(1, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(1, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(1, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(1, "_diffuseMap", "tex:jiashan001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(2, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(2, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(2, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(2, "_diffuseMap", "tex:wapian002.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(3, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(3, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(3, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(3, "_diffuseMap", "tex:huaban001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(4, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(4, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(4, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(4, "_diffuseMap", "tex:shui.jpg" );

		pActor->AttachComponent( pMeshRenderComponent.upcast<Component>() );
		
		return pActor;		
	}

	App::TActorPtr DrawCJ_diji2( const Util::String& sActorName/*="House1"*/ )
	{
		using namespace App;

		App::TActorPtr pActor = Actor::Create();
		pActor->SetName( sActorName );

		

		Ptr<MeshRenderComponent> pMeshRenderComponent = MeshRenderComponent::Create();
		pMeshRenderComponent->SetMeshID( "msh:cj_diji2.mesh" );
		pMeshRenderComponent->SetMaterialByShaderID(0, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(0, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(0, "_diffuseMap", "tex:zhuan002.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(1, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(1, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(1, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(1, "_diffuseMap", "tex:zhuan001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(2, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(2, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(2, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(2, "_diffuseMap", "tex:mutou007.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(3, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(3, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(3, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(3, "_diffuseMap", "tex:mutou004.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(4, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(4, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(4, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(4, "_diffuseMap", "tex:qiang003.jpg" );

		pActor->AttachComponent( pMeshRenderComponent.upcast<Component>() );

		return pActor;		
	}

	App::TActorPtr DrawCJ_diji4( const Util::String& sActorName/*="House1"*/ )
	{
		using namespace App;

		App::TActorPtr pActor = Actor::Create();
		pActor->SetName( sActorName );
		

		Ptr<MeshRenderComponent> pMeshRenderComponent = MeshRenderComponent::Create();
		pMeshRenderComponent->SetMeshID( "msh:cj_diji4.mesh" );
		pMeshRenderComponent->SetMaterialByShaderID(0, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(0, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(0, "_diffuseMap", "tex:zhuan002.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(1, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(1, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(1, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(1, "_diffuseMap", "tex:zhuan001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(2, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(2, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(2, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(2, "_diffuseMap", "tex:mutou004.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(3, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(3, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(3, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(3, "_diffuseMap", "tex:mutou003.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(4, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(4, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(4, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(4, "_diffuseMap", "tex:mutou007.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(5, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(5, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(5, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(5, "_diffuseMap", "tex:mutou001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(6, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(6, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(6, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(6, "_diffuseMap", "tex:mutou002.jpg" );

		pActor->AttachComponent( pMeshRenderComponent.upcast<Component>() );

		return pActor;		
	}

	App::TActorPtr DrawCJ_juanxincai002( const Util::String& sActorName/*="House1"*/ )
	{
		using namespace App;

		App::TActorPtr pActor = Actor::Create();
		pActor->SetName( sActorName );

		


		Ptr<MeshRenderComponent> pMeshRenderComponent = MeshRenderComponent::Create();
		pMeshRenderComponent->SetMeshID( "msh:cj_juanxincai002.mesh" );
		pMeshRenderComponent->SetMaterialByShaderID(0, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(0, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(0, "_diffuseMap", "tex:guanmu_juanxincai.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(1, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(1, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(1, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(1, "_diffuseMap", "tex:juanxincai1.jpg" );

		pActor->AttachComponent( pMeshRenderComponent.upcast<Component>() );

		return pActor;		
	}

	App::TActorPtr DrawCJ_jiulou001( const Util::String& sActorName/*="House1"*/ )
	{
		using namespace App;

		App::TActorPtr pActor = Actor::Create();
		pActor->SetName( sActorName );

		


		Ptr<MeshRenderComponent> pMeshRenderComponent = MeshRenderComponent::Create();
		pMeshRenderComponent->SetMeshID( "msh:cj_jiulou001.mesh" );
		pMeshRenderComponent->SetMaterialByShaderID(0, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(0, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(0, "_diffuseMap", "tex:zhuan002.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(1, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(1, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(1, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(1, "_diffuseMap", "tex:zhuan001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(2, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(2, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(2, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(2, "_diffuseMap", "tex:qiang003.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(3, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(3, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(3, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(3, "_diffuseMap", "tex:mutou007.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(4, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(4, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(4, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(4, "_diffuseMap", "tex:mutou008.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(5, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(5, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(5, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(5, "_diffuseMap", "tex:mutou005.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(6, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(6, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(6, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(6, "_diffuseMap", "tex:mutou006.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(7, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(7, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(7, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(7, "_diffuseMap", "tex:mutou001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(8, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(8, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(8, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(8, "_diffuseMap", "tex:mutou002.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(9, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(9, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(9, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(9, "_diffuseMap", "tex:mutou003.jpg" );

		//--
		pMeshRenderComponent->SetMaterialByShaderID(10, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(10, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(10, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(10, "_diffuseMap", "tex:mutou004.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(11, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(11, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(11, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(11, "_diffuseMap", "tex:wapian002.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(12, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(12, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(12, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(12, "_diffuseMap", "tex:wapian001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(13, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(13, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(13, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(13, "_diffuseMap", "tex:mutou0013.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(14, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(14, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(14, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(14, "_diffuseMap", "tex:Standard_12.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(15, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(15, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(15, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(15, "_diffuseMap", "tex:huangzi001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(16, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(16, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(16, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(16, "_diffuseMap", "tex:jiutan002.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(17, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(17, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(17, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(17, "_diffuseMap", "tex:jiutan001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(18, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(18, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(18, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(18, "_diffuseMap", "tex:xiong001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(19, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(19, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(19, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(19, "_diffuseMap", "tex:menlian001.jpg" );

		
		pActor->AttachComponent( pMeshRenderComponent.upcast<Component>() );

		return pActor;		
	}


	App::TActorPtr DrawCJ_jzz2( const Util::String& sActorName/*="House1"*/ )
	{
		using namespace App;

		App::TActorPtr pActor = Actor::Create();
		pActor->SetName( sActorName );

		


		Ptr<MeshRenderComponent> pMeshRenderComponent = MeshRenderComponent::Create();
		pMeshRenderComponent->SetMeshID( "msh:cj_jzz2.mesh" );
		pMeshRenderComponent->SetMaterialByShaderID(0, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(0, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(0, "_diffuseMap", "tex:zhuan002.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(1, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(1, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(1, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(1, "_diffuseMap", "tex:zhuan001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(2, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(2, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(2, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(2, "_diffuseMap", "tex:mutou004.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(3, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(3, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(3, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(3, "_diffuseMap", "tex:mutou003.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(4, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(4, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(4, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(4, "_diffuseMap", "tex:mutou005.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(5, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(5, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(5, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(5, "_diffuseMap", "tex:wapian001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(6, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(6, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(6, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(6, "_diffuseMap", "tex:mutou002.jpg" );

		
		pActor->AttachComponent( pMeshRenderComponent.upcast<Component>() );

		return pActor;		
	}


	App::TActorPtr DrawCJ_jzz4( const Util::String& sActorName/*="House1"*/ )
	{
		using namespace App;

		App::TActorPtr pActor = Actor::Create();
		pActor->SetName( sActorName );

		


		Ptr<MeshRenderComponent> pMeshRenderComponent = MeshRenderComponent::Create();
		pMeshRenderComponent->SetMeshID( "msh:cj_jzz4.mesh" );
		pMeshRenderComponent->SetMaterialByShaderID(0, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(0, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(0, "_diffuseMap", "tex:zhuan002.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(1, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(1, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(1, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(1, "_diffuseMap", "tex:zhuan001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(2, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(2, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(2, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(2, "_diffuseMap", "tex:mutou005.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(3, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(3, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(3, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(3, "_diffuseMap", "tex:mutou004.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(4, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(4, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(4, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(4, "_diffuseMap", "tex:wapian001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(5, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(5, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(5, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(5, "_diffuseMap", "tex:mutou002.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(6, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(6, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(6, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(6, "_diffuseMap", "tex:mutou003.jpg" );

		
		pActor->AttachComponent( pMeshRenderComponent.upcast<Component>() );

		return pActor;		
	}


	App::TActorPtr DrawCJ_matou001( const Util::String& sActorName/*="House1"*/ )
	{
		using namespace App;

		App::TActorPtr pActor = Actor::Create();
		pActor->SetName( sActorName );

		


		Ptr<MeshRenderComponent> pMeshRenderComponent = MeshRenderComponent::Create();
		pMeshRenderComponent->SetMeshID( "msh:cj_matou001.mesh" );
		pMeshRenderComponent->SetMaterialByShaderID(0, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(0, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(0, "_diffuseMap", "tex:cj_matou001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(1, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(1, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(1, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(1, "_diffuseMap", "tex:wa002.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(2, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(2, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(2, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(2, "_diffuseMap", "tex:matou_zhuan002.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(3, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(3, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(3, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(3, "_diffuseMap", "tex:zhuan003.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(4, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(4, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(4, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(4, "_diffuseMap", "tex:shitou001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(5, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(5, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(5, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(5, "_diffuseMap", "tex:qiang001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(6, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(6, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(6, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(6, "_diffuseMap", "tex:wa001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(7, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(7, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(7, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(7, "_diffuseMap", "tex:matou_mutou001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(8, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(8, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(8, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(8, "_diffuseMap", "tex:mutou002.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(9, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(9, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(9, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(9, "_diffuseMap", "tex:tulu004.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(10, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(10, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(10, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(10, "_diffuseMap", "tex:zhuan004.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(11, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(11, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(11, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(11, "_diffuseMap", "tex:zhuan001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(12, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(12, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(12, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(12, "_diffuseMap", "tex:matou_wapian001.jpg" );

		
		pActor->AttachComponent( pMeshRenderComponent.upcast<Component>() );

		return pActor;		
	}

	App::TActorPtr DrawCJ_daozi002( const Util::String& sActorName/*="House1"*/ )
	{
		using namespace App;

		App::TActorPtr pActor = Actor::Create();
		pActor->SetName( sActorName );

		


		Ptr<MeshRenderComponent> pMeshRenderComponent = MeshRenderComponent::Create();
		pMeshRenderComponent->SetMeshID( "msh:daozi002.mesh" );
		pMeshRenderComponent->SetMaterialByShaderID(0, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(0, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(0, "_diffuseMap", "tex:daozi003.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(1, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(1, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(1, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(1, "_diffuseMap", "tex:daozi.jpg" );
		
		pMeshRenderComponent->SetMaterialByShaderID(2, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(2, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(2, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(2, "_diffuseMap", "tex:daozi001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(3, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(3, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(3, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(3, "_diffuseMap", "tex:tudi.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(4, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(4, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(4, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(4, "_diffuseMap", "tex:cj_caoditouying001.jpg" );

		
		pActor->AttachComponent( pMeshRenderComponent.upcast<Component>() );

		return pActor;		
	}

	App::TActorPtr DrawCJ_huanggua001( const Util::String& sActorName/*="House1"*/ )
	{
		using namespace App;

		App::TActorPtr pActor = Actor::Create();
		pActor->SetName( sActorName );

		


		Ptr<MeshRenderComponent> pMeshRenderComponent = MeshRenderComponent::Create();
		pMeshRenderComponent->SetMeshID( "msh:huanggua001.mesh" );
		pMeshRenderComponent->SetMaterialByShaderID(0, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(0, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(0, "_diffuseMap", "tex:huangguaye1.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(1, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(1, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(1, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(1, "_diffuseMap", "tex:huangguaye2.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(2, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(2, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(2, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(2, "_diffuseMap", "tex:huangguateng.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(3, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(3, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(3, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(3, "_diffuseMap", "tex:tudi.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(4, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(4, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(4, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(4, "_diffuseMap", "tex:cj_caoditouying001.jpg" );

		
		pActor->AttachComponent( pMeshRenderComponent.upcast<Component>() );

		return pActor;		
	}

	App::TActorPtr DrawCJ_huanggua002( const Util::String& sActorName/*="House1"*/ )
	{
		using namespace App;

		App::TActorPtr pActor = Actor::Create();
		pActor->SetName( sActorName );

		


		Ptr<MeshRenderComponent> pMeshRenderComponent = MeshRenderComponent::Create();
		pMeshRenderComponent->SetMeshID( "msh:huanggua002.mesh" );
		pMeshRenderComponent->SetMaterialByShaderID(0, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(0, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(0, "_diffuseMap", "tex:kuweigao.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(1, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(1, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(1, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(1, "_diffuseMap", "tex:huangguaye11.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(2, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(2, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(2, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(2, "_diffuseMap", "tex:huangguateng.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(3, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(3, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(3, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(3, "_diffuseMap", "tex:huangguateng.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(4, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(4, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(4, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(4, "_diffuseMap", "tex:tudi.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(5, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(5, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(5, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(5, "_diffuseMap", "tex:cj_caoditouying001.jpg" );

		
		pActor->AttachComponent( pMeshRenderComponent.upcast<Component>() );

		return pActor;		
	}

	App::TActorPtr DrawCJ_kuwei001( const Util::String& sActorName/*="House1"*/ )
	{
		using namespace App;

		App::TActorPtr pActor = Actor::Create();
		pActor->SetName( sActorName );

		


		Ptr<MeshRenderComponent> pMeshRenderComponent = MeshRenderComponent::Create();
		pMeshRenderComponent->SetMeshID( "msh:kuwei001.mesh" );
		pMeshRenderComponent->SetMaterialByShaderID(0, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(0, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(0, "_diffuseMap", "tex:kuweigao.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(1, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(1, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(1, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(1, "_diffuseMap", "tex:tudi.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(2, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(2, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(2, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(2, "_diffuseMap", "tex:cj_caoditouying001.jpg" );

		
		pActor->AttachComponent( pMeshRenderComponent.upcast<Component>() );

		return pActor;		
	}

	App::TActorPtr DrawCJ_tiandi( const Util::String& sActorName/*="House1"*/ )
	{
		using namespace App;

		App::TActorPtr pActor = Actor::Create();
		pActor->SetName( sActorName );

		


		Ptr<MeshRenderComponent> pMeshRenderComponent = MeshRenderComponent::Create();
		pMeshRenderComponent->SetMeshID( "msh:tiandi.mesh" );
		pMeshRenderComponent->SetMaterialByShaderID(0, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(0, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(0, "_diffuseMap", "tex:tudi.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(1, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(1, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(1, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(1, "_diffuseMap", "tex:cj_caoditouying001.jpg" );

		
		pActor->AttachComponent( pMeshRenderComponent.upcast<Component>() );

		return pActor;		
	}

	App::TActorPtr DrawCJ_nangua( const Util::String& sActorName/*="House1"*/ )
	{
		using namespace App;

		App::TActorPtr pActor = Actor::Create();
		pActor->SetName( sActorName );

		


		Ptr<MeshRenderComponent> pMeshRenderComponent = MeshRenderComponent::Create();
		pMeshRenderComponent->SetMeshID( "msh:nangua.mesh" );
		pMeshRenderComponent->SetMaterialByShaderID(0, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(0, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(0, "_diffuseMap", "tex:nangua.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(1, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(1, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(1, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(1, "_diffuseMap", "tex:juanxincai.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(2, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(2, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(2, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(2, "_diffuseMap", "tex:tudi.jpg" );

		
		pActor->AttachComponent( pMeshRenderComponent.upcast<Component>() );

		return pActor;		
	}

	App::TActorPtr DrawCJ_minju001( const Util::String& sActorName/*="House1"*/ )
	{
		using namespace App;

		App::TActorPtr pActor = Actor::Create();
		pActor->SetName( sActorName );

		
		

		Ptr<MeshRenderComponent> pMeshRenderComponent = MeshRenderComponent::Create();
		pMeshRenderComponent->SetMeshID( "msh:cj_minju001.mesh" );
		pMeshRenderComponent->SetMaterialByShaderID(0, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(0, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(0, "_diffuseMap", "tex:dimian001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(1, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(1, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(1, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(1, "_diffuseMap", "tex:qiang002.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(2, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(2, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(2, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(2, "_diffuseMap", "tex:qiang004.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(3, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(3, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(3, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(3, "_diffuseMap", "tex:shitou001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(4, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(4, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(4, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(4, "_diffuseMap", "tex:shitou002.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(5, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(5, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(5, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(5, "_diffuseMap", "tex:mutou002.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(6, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(6, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(6, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(6, "_diffuseMap", "tex:mutou001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(7, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(7, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(7, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(7, "_diffuseMap", "tex:qiang001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(8, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(8, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(8, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(8, "_diffuseMap", "tex:qiang003.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(9, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(9, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(9, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(9, "_diffuseMap", "tex:mutou008.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(10, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(10, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(10, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(10, "_diffuseMap", "tex:wapian001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(11, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(11, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(11, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(11, "_diffuseMap", "tex:wapian002.jpg" );

		
		pActor->AttachComponent( pMeshRenderComponent.upcast<Component>() );

		return pActor;		
	}


	App::TActorPtr DrawCJ_mxzgh001( const Util::String& sActorName/*="House1"*/ )
	{
		using namespace App;

		App::TActorPtr pActor = Actor::Create();
		pActor->SetName( sActorName );

		


		Ptr<MeshRenderComponent> pMeshRenderComponent = MeshRenderComponent::Create();
		pMeshRenderComponent->SetMeshID( "msh:cj_mxzgh001.mesh" );
		pMeshRenderComponent->SetMaterialByShaderID(0, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(0, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(0, "_diffuseMap", "tex:dimian_mx.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(1, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(1, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(1, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(1, "_diffuseMap", "tex:wapian_mx.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(2, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(2, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(2, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(2, "_diffuseMap", "tex:wapian002.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(3, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(3, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(3, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(3, "_diffuseMap", "tex:mutou005.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(4, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(4, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(4, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(4, "_diffuseMap", "tex:tuteng001_mx.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(5, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(5, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(5, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(5, "_diffuseMap", "tex:mutou004.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(6, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(6, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(6, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(6, "_diffuseMap", "tex:qiang003.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(7, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(7, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(7, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(7, "_diffuseMap", "tex:zhuan001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(8, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(8, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(8, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(8, "_diffuseMap", "tex:zhaopai_mx.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(9, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(9, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(9, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(9, "_diffuseMap", "tex:mutou007.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(10, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(10, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(10, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(10, "_diffuseMap", "tex:dimian111.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(11, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(11, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(11, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(11, "_diffuseMap", "tex:zhuan002.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(12, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(12, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(12, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(12, "_diffuseMap", "tex:wapian00212_mx.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(13, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(13, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(13, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(13, "_diffuseMap", "tex:xiong001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(14, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(14, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(14, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(14, "_diffuseMap", "tex:bu001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(15, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(15, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(15, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(15, "_diffuseMap", "tex:mutou003.jpg" );

		
		pActor->AttachComponent( pMeshRenderComponent.upcast<Component>() );

		return pActor;		
	}


	App::TActorPtr DrawCJ_simiao002( const Util::String& sActorName/*="House1"*/ )
	{
		using namespace App;

		App::TActorPtr pActor = Actor::Create();
		pActor->SetName( sActorName );

		
		

		Ptr<MeshRenderComponent> pMeshRenderComponent = MeshRenderComponent::Create();
		pMeshRenderComponent->SetMeshID( "msh:cj_simiao002.mesh" );
		pMeshRenderComponent->SetMaterialByShaderID(0, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(0, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(0, "_diffuseMap", "tex:jinshu001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(1, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(1, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(1, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(1, "_diffuseMap", "tex:zhuan001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(2, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(2, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(2, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(2, "_diffuseMap", "tex:jiutan001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(3, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(3, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(3, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(3, "_diffuseMap", "tex:mutou006.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(4, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(4, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(4, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(4, "_diffuseMap", "tex:yu.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(5, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(5, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(5, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(5, "_diffuseMap", "tex:mutou005.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(6, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(6, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(6, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(6, "_diffuseMap", "tex:dimian111.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(7, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(7, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(7, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(7, "_diffuseMap", "tex:xiong001.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(8, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(8, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(8, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(8, "_diffuseMap", "tex:wapian.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(9, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(9, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(9, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(9, "_diffuseMap", "tex:wapian002.jpg" );

		
		pActor->AttachComponent( pMeshRenderComponent.upcast<Component>() );

		return pActor;		
	}

	App::TActorPtr DrawCJ_zhuzi( const Util::String& sActorName/*="House1"*/ )
	{
		using namespace App;

		App::TActorPtr pActor = Actor::Create();
		pActor->SetName( sActorName );

		
		

		Ptr<MeshRenderComponent> pMeshRenderComponent = MeshRenderComponent::Create();
		pMeshRenderComponent->SetMeshID( "msh:zhuzi.mesh" );
		pMeshRenderComponent->SetMaterialByShaderID(0, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(0, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(0, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(0, "_diffuseMap", "tex:zuoye.jpg" );

		pMeshRenderComponent->SetMaterialByShaderID(1, "shd:simple.wjshader");
		pMeshRenderComponent->_AddTextureParam(1, "_diffuseMap", "sys:white.dds");
		n_assert( pMeshRenderComponent->HasTextureParam(1, "_diffuseMap") );
		pMeshRenderComponent->SetTexture(1, "_diffuseMap", "tex:zhuzi.jpg" );

		
		pActor->AttachComponent( pMeshRenderComponent.upcast<Component>() );

		return pActor;		
	}



}