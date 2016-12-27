/** \file: GameData.h
* \brief: 
* Copyright (c) 2011,WebJet Enterprise Department.ChangYou
* All rights reserved.
* Date        Ver    Who          Comment
* 2011/12/03   1.0    SunHao   
*/

#ifndef __GAMEDATA_H__
#define __GAMEDATA_H__

#include "core/ptr.h"
#include "core/refcounted.h"
#include "core/singleton.h"

namespace Demo
{
	// forward declare
	class BuildingData;
	// typedef
	typedef Ptr<BuildingData> TBuildingDataPtr;

	class BuildingData: public Core::RefCounted
	{
		__DeclareSubClass( Demo::BuildingData, Core::RefCounted)
	public:
		BuildingData()
			: m_sTemplateName()
			, m_iMeasure( 0 )
		{}
		
		~BuildingData()
		{}

		void SetData( const Util::String& name, int iMeasure );
		const Util::String& TemplateName( void ) { return m_sTemplateName; }
		int Measure( void ) { return m_iMeasure; }

	private:
		Util::String m_sTemplateName;			///< -  Template name store in atp file
		int m_iMeasure;							///< -	how many grids this building takes 	
	};

	class BuildingDataSet
	{
	public:
		BuildingDataSet();
		~BuildingDataSet();

		bool LoadData( void );

		TBuildingDataPtr BuildingData( SizeT iIndex );
	private:
		void Serialization( void );

		Util::String m_sFilePath;
		Util::Array<TBuildingDataPtr> m_Data;
	};

	class GameData: public Core::RefCounted
	{
		__DeclareSubClass( Demo::GameData, Core::RefCounted )
		__DeclareThreadSingleton( Demo::GameData )
	public:
		GameData();
		~GameData();
		// -------- LoadData --------------------------------
		bool LoadData( void );

		// -------- BuildingData --------------------------------
		TBuildingDataPtr BuildingData( SizeT iIndex );
	private:
		BuildingDataSet m_BuildingData;				
	};
}

#endif // __GAMEDATA_H__