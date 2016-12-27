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
#include "graphicfeature/components/skeletoncomponent.h"
#include "graphicfeature/components/animationcomponent.h"
#include "graphicfeature/graphicsfeature.h"
#include "graphicfeature/graphicsfeatureprotocol.h"
#include "resource/resourceserver.h"
#include "serialization/serializeserver.h"

namespace App
{
	using namespace Graphic;
	using namespace Resources;

	__ImplementClass(SkeletonComponent, 'SKCM', App::Component);

	SkeletonComponent::SkeletonComponent()
		:  m_pSkelResInfo(NULL),
		m_pEmptyRes(NULL),
		m_bSkeletonDirty(false),
		m_bHasSkeletonUpdateToSkin(false),
		m_nPriority(Resources::ResourcePriority::SkeletonDefault)
	{

	}

	SkeletonComponent::~SkeletonComponent()
	{
		if (m_pSkelResInfo.isvalid())
		{
			m_pSkelResInfo = NULL;
		}
	}

	void SkeletonComponent::OnActivate()
	{
		Actor::Check_Skeleton(this);
		Super::OnActivate();
	}

	void SkeletonComponent::OnDeactivate()
	{
		Super::OnDeactivate();
	}

	void SkeletonComponent::SetupCallbacks()
	{
		mActor->RegisterComponentCallback(this, BeginFrame);
	}

	void SkeletonComponent::SetupAcceptedMessages()
	{
		Super::SetupAcceptedMessages();
	}

	void SkeletonComponent::HandleMessage(const GPtr<Messaging::Message>& msg)
	{
		Super::HandleMessage(msg);
	}

	void SkeletonComponent::SetSkeletonID(const Resources::ResourceId &skeletonID, IndexT priority)
	{
		if (m_SkelID != skeletonID)
		{
			m_SkelID = skeletonID;

			if (m_pSkelResInfo.isvalid())
			{
				m_pSkelResInfo   = NULL;
			}
			if (mActor && mActor->PriorityDefinition())
			{
				priority = mActor->GetPriority();
			}
			m_nPriority      = priority;

		}
		m_bSkeletonDirty = true;
		m_bHasSkeletonUpdateToSkin = false;
	}

	void SkeletonComponent::_OnBeginFrame()
	{
		if (m_bSkeletonDirty)
		{
			if ( m_pSkelResInfo.isvalid() )
			{
				if (m_pSkelResInfo->GetRes().isvalid())
				{
					SetAnimSkelTree();
				}
			}
			else
			{
				m_pSkelResInfo = Resources::ResourceManager::Instance()->CreateSkeletonResInfo(m_SkelID, m_nPriority);
			}
		}


	}

#ifdef __GENESIS_EDITOR__
	bool IsModelContainSkelInfo(const GPtr<Actor>& rootActor, const Resources::ResourceId& skelId)
	{
		Util::String modelName = rootActor->GetModelName();

		if(modelName == "")
			return true;

		GPtr<IO::Stream> pStream = IO::MemoryStream::Create();
		n_assert( pStream );

		GPtr<IO::ReadStream> readStreamMsg = IO::ReadStream::Create();
		n_assert( readStreamMsg );

		IO::AssignRegistry* pAssignRegistry = IO::AssignRegistry::Instance();

		IO::URI resolvedPath = pAssignRegistry->ResolveAssigns(modelName);
		if(!resolvedPath.IsValid())
			return true;

		Util::String resPath = resolvedPath.LocalPath();

		Serialization::SerializationServer* serialize = Serialization::SerializationServer::Instance();
		GPtr<Serialization::SerializeReader> pReader = serialize->OpenReadFile( resPath,Serialization::FT_DEFAULT );
		if ( pReader )
		{
			try
			{
				GPtr<Actor> actor;
				actor = pReader->SerializeObject<Actor>();

				if (!actor.isvalid())
				{
					serialize->CloseReadFile(pReader);
					return false;
				}

				bool result = false;
				// 临时代码：model文件里只有一个actor关节一个子actor
				if (actor->GetChildCount() != 0)
				{
					for(int i=0; i<actor->GetChildCount(); i++)
					{
						GPtr<Actor> targetActor = NULL;
						targetActor = actor->GetChild(i);
						if (!targetActor.isvalid())
						{
							continue;
						}
						GPtr<Component> com = targetActor->FindComponent(SkeletonComponent::RTTI);
						if (!com.isvalid())
						{
							continue;
						}
						
						GPtr<SkeletonComponent> skelCom = com.downcast<SkeletonComponent>();
						
						const Resources::ResourceId& skelResId = skelCom->GetSkeletonID();
						
						if(skelCom->GetSkeletonID() == skelId)
						{
							result = true;
							break;
						}
					}
				}
				
				actor->Deactive(true);
				actor->Destory(true);
				actor = NULL;

				serialize->CloseReadFile(pReader);
				return result;
			}
			catch(...)
			{
				serialize->CloseReadFile(pReader);
			}
		}

		return false;
	}
#endif

	void SkeletonComponent::SetAnimSkelTree()
	{
		//Find animation data
		Actor* actorParent = mActor->GetParent();
		if (!actorParent)
		{
			return;
		}

		GPtr<Component> animation = actorParent->FindComponent(AnimationComponent::RTTI);
		if(animation.isvalid())
		{
			GPtr<AnimationComponent> pAnimationCom = animation.downcast<AnimationComponent>();

			const GPtr<SkeletonRes>& skeletonRes = GetSkeleton();
			if (skeletonRes.isvalid() && !skeletonRes->IsSkelTreeEmpty())
			{

#ifdef __GENESIS_EDITOR__
				if(!IsModelContainSkelInfo(actorParent, GetSkeletonID()))
				{
					m_bSkeletonDirty = false;
					return;
				}
#endif

				pAnimationCom->SetSkelTree(skeletonRes->GetSkelTree());
				pAnimationCom->SetSkelTree(skeletonRes->GetSkelTreeArray());

				pAnimationCom->SetGetSkelTree(true);

				m_bSkeletonDirty = false;
			}
		}

	}

	//------------------------------------------------------------------------
	void SkeletonComponent::CopyFrom( const GPtr<Component>& pComponent )
	{
		if( !pComponent.isvalid()  )
			return;
		if( !pComponent->GetRtti()->IsDerivedFrom( *(this->GetRtti()) ) )
			return;
		// TODO:

		GPtr<SkeletonComponent> pSource = pComponent.downcast<SkeletonComponent>();
		n_assert(pSource.isvalid());

		SetSkeletonID(pSource->GetSkeletonID());
	}

	void SkeletonComponent::SetupAllResource()
	{
		if (mActor->PriorityUndefinition())
			return;

		SetSkeletonID(m_SkelID,mActor->GetPriority());
	}

	bool SkeletonComponent::IsAllResourceLoaded()
	{
		return IsAllLoaded();
	}

	void SkeletonComponent::GetReferenceResourceId(Util::Array<Resources::ReferenceResource>& list) const
	{
		list.Append(Resources::ReferenceResource(m_SkelID, Resources::RR_Unknown));
		Super::GetReferenceResourceId(list);
	}

}