/** \file: GameLogic.h
* \brief: Demo game logic
* Copyright (c) 2011,WebJet Enterprise Department.ChangYou
* All rights reserved.
* Date        Ver    Who          Comment
* 2011/11/17   1.0    SunHao   
*/

#ifndef __GAMELOGIC_H__
#define __GAMELOGIC_H__

#include "util/dictionary.h"
#include "core/refcounted.h"
#include "core/singleton.h"
#include "math/float3.h"
#include "appframework/actor.h"
#include "appframework/app_fwd_decl.h"
#include "GameBuilding.h"
#include "addons/serialization/serialize.h"

namespace Demo
{
	// - forward declare
	class GameLogic;

	// store the mouse state
	class MouseState
	{
	public:
		enum
		{
			eMouseState_Normal,
			eMouseState_Building,
			eMouseState_Num
		};

		MouseState()
			: m_eState( eMouseState_Normal )
		{}
		~MouseState()
		{}

		void SetState( int32 iState );
		int32 GetState( void );
	private:
		int32 m_eState;

	};

	class GridIndx
	{
	public:
		GridIndx()
			: m_iRow( 0 )
			, m_iCol( 0 )
		{}

		GridIndx( uint16 iRow, uint16 iCol )
			: m_iRow( iRow )
			, m_iCol( iCol )
		{}

		uint16 Row( void )const { return m_iRow; }
		uint16 Col( void )const { return m_iCol; }

		bool operator<(const GridIndx& rhs) const
		{
			if ( m_iRow<rhs.Row() )
			{
				return true; 
			}
			else if ( m_iRow>rhs.Row() )
			{
				return false;
			}
			else
			{
				return m_iCol<rhs.Col();
			}
		}

		bool operator==( const GridIndx& rhs )const 
		{
			if ( m_iRow==rhs.Row() &&
				m_iCol==rhs.Col() )
			{
				return true;
			}
			return false;
		}

		bool operator!=( const GridIndx& rhs )const 
		{
			return !( this->operator==(rhs) );
		}

		bool operator>(const GridIndx& rhs )const
		{
			if ( m_iRow>rhs.Row() )
			{
				return true; 
			}
			else if ( m_iRow<rhs.Row() )
			{
				return false;
			}
			else
			{
				return m_iCol>rhs.Col();
			}
			//return !(this->operator<(rhs));
		}

	private:
		uint16 m_iRow;
		uint16 m_iCol;
	};

	// show grids on the ground
	class Grids
	{
		typedef GridIndx TGridIndex;
	public:
		Grids();
		~Grids();
		void Init( float fLen, float fYpos, int iGridCount );
		
		void ShowGirds( void );
		void HideGirds( void );

		SizeT Size( void );
		void Clear( void );

		// - calculate where the building can be build, if success a none pos is return, if not, return zero
		Math::float3 IsCanBuild( const Math::float3& pos, int iMeasure );
		
		App::Actor::FastId GetActorId( Math::float3 pos );
		// - 
		bool Build( App::Actor::FastId iActorId );
		void Remove( App::Actor::FastId iActorId );

	private:
		void ResetSelectRecord( void );

	private:
		float m_fHalfLen;
		float m_fYPos;
		int   m_iGridCount;

		Math::float3 m_vertex0;
		Math::float3 m_vertex1;
		Math::float3 m_vertex2;
		Math::float3 m_vertex3;

		App::TActorPtr m_Grids;
		Util::Array<TGridIndex> m_CalcResult;
		Util::Array<Util::Array<bool>> m_selectRecord; 
		Util::Dictionary<TGridIndex, App::Actor::FastId> m_GridRegist;
	};

	// display shortcut
	class Preview
	{
	public:
		Preview()
			: m_pActor()
		{}

		~Preview()
		{
			if ( m_pActor.isvalid() )
			{
				m_pActor->Destory();
			}
		}

		void Create( int iBuildingType );
		void SetPos( const Math::vector& pos );
		void ShowPreView( void );
		void HidePreView( void );

	private:
		App::TActorPtr m_pActor;
	};

	class LogicSerialization : public Core::RefCounted
	{
		__DeclareSubClass( Demo::LogicSerialization, Core::RefCounted )
	public:
		LogicSerialization()
			: m_pGame( NULL )
			, m_Lands()
		{}

		~LogicSerialization(){}

		void SetGame( GameLogic* pGame );
		void LoadGame( GameLogic* pGame );

		App::Version GetVersion() const;
		void Load( App::Version ver, App::AppReader* pReader );
		void Save( App::AppWriter* pWriter ) const;

	private:
		GameLogic* m_pGame;
		Util::Array<TEstatesPtr> m_Lands;
	};

	class GameLogic : public Core::RefCounted
	{
		__DeclareSubClass( Demo::GameLogic, Core::RefCounted );
		__DeclareThreadSingleton( Demo::GameLogic )
	public:
		GameLogic();
		~GameLogic();

		void GameInit( void );
		// --------------------------- actor selected -----------------------------------
		void Select( Math::float3 intersectPoint );
		void RemoveSelected( void );
		void CancelSelected( void );
		App::TActorPtr GetSelected( void );

		// --------------------------- actor selected -----------------------------------
		void RotateSelectedBuilding( Math::scalar radian );

		// --------------------------- scene name -----------------------------------
		void SetSceneName( const Util::String& sName );
		const Util::String& GetSceneName( void );

		// --------------------------- mouse state -----------------------------------		
		void SetMouseState( int32 iState );
		void SetMouseNormal( void );
		int32 GetMouseState( void );

		// ---------------------------- preview -------------------------------------
		void ShowPreview( const Math::vector& pos );

		// ----------------------------- build ---------------------------------------
		void PrepareToBuild( int iType );
		void BuildOnGrids( Math::float3 pos );
		void BuildOnGrids( Math::float3 pos, Math::scalar radian );
		void CancelPreparedBuild( void );

		// ---------------------------- grid -----------------------------------------
		void ShowGirds( void );
		void HideGirds( void );

		// ---------------------------- update -----------------------------------------
		void Update( float32 fTime=0.f );

		// ---------------------------- scene ------------------------------------------
		void ToggleScene( void );
		void SaveScene( void );

		// ---------------------------- serialization --------------------------------
		void SaveLogic( void );
		void LoadLogic( const Util::String& sFileName );

		// ------------------------ system ------------------------------------
		void Restart( void );
		static bool GameStart( void );
		static bool GameClose(void);

		// ---- friend class define 
		friend class LogicSerialization;
	private:
		// ----------------------------- build ---------------------------------------
		App::TActorPtr Build(  Math::float3 pos );

	private:
		Util::String m_sSceneName;			///< - current scene name

		MouseState m_MouseState;			///< - record mouse state

		LandAgent m_landAgent;				///< - manage all the real eatates

		Preview m_Preview;					///< - show short cut when build

		App::Actor::FastId m_SelectedId;	///< - selected actor id

		int m_iBuildingType;				///< - building type which is prepare to build

		Grids m_Grids;						///< - show grids when building

		Ptr<BuildingAnimationManager> m_AnimMgr;	///< - contorl the build growth animtion
	};

	// - MouseState ------------------------------------------------
	inline int32 MouseState::GetState( void )
	{
		return m_eState;
	}

	// - GameLogic -------------------------------------------------
	inline void GameLogic::SetSceneName( const Util::String& sName )
	{
		this->m_sSceneName = sName;
	}

	inline const Util::String& GameLogic::GetSceneName( void )
	{
		return this->m_sSceneName;
	}

	inline int32 GameLogic::GetMouseState( void )
	{
		return m_MouseState.GetState();
	}

}

#endif // __GAMELOGIC_H__