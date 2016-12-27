/** \file: GameData.cc
* \brief: 
* Copyright (c) 2011,WebJet Enterprise Department.ChangYou
* All rights reserved.
* Date        Ver    Who          Comment
* 2011/12/03   1.0    SunHao   
*/
#include "stdneb.h"
#include "core/rttimacros.h"
#include "io/memorystream.h"
#include "io/xmlreader.h"
#include "io/iointerfaceprotocol.h"
#include "io/iointerface.h"

#include "GameData.h"

namespace Demo
{
	// - Building data implement ------------------------------------------------------
	__ImplementClass( Demo::BuildingData, 'DMDA', Core::RefCounted )

	void BuildingData::SetData( const Util::String& name, int iMeasure )
	{
		m_sTemplateName = name;
		m_iMeasure = iMeasure;
	}

	// - BuildingDataSet implement ----------------------------------------------------
	BuildingDataSet::BuildingDataSet()
		: m_sFilePath( "logic:BuildingData.glogic" )
		, m_Data()
	{
		// make the index starts from one
		TBuildingDataPtr pEmpty;
		m_Data.Append( pEmpty );
	}

	BuildingDataSet::~BuildingDataSet()
	{
		//empty
	}

	bool BuildingDataSet::LoadData( void )
	{
		// load from file
		Ptr<IO::Stream> pStream = IO::MemoryStream::Create();
		n_assert( pStream );

		Ptr<IO::ReadStream> readStreamMsg = IO::ReadStream::Create();
		n_assert( readStreamMsg );

		readStreamMsg->SetURI( m_sFilePath );
		readStreamMsg->SetStream( pStream );

		IO::IoInterface::Instance()->SendWait( readStreamMsg.upcast<Messaging::Message>() );

		if ( !readStreamMsg->GetResult() )
		{
			n_error( "BuildingDataSet::BuildingDataSet, can not open %s", m_sFilePath.AsCharPtr() );
		}

		// - parser data
		Ptr<IO::XmlReader> reader = IO::XmlReader::Create();
		reader->SetStream( pStream );

		if ( !reader->Open() )
		{
			n_error( "BuildingDataSet::BuildingDataSet, Parser data error,file %s", m_sFilePath.AsCharPtr() );
		}

		if( reader->GetCurrentNodeName( )=="Model")
		{
			if ( reader->SetToFirstChild("BuildingDataSet") )
			{
				Util::String sTmplName;
				sTmplName = reader->GetString( "TemplateName" );
				
				int iMeasure;
				iMeasure = reader->GetInt( "Measure" );

				TBuildingDataPtr pBuildingData = BuildingData::Create();
				pBuildingData->SetData( sTmplName, iMeasure );
				m_Data.Append( pBuildingData );

				while( reader->SetToNextChild("BuildingDataSet") )
				{
					sTmplName = reader->GetString( "TemplateName" );
					iMeasure = reader->GetInt( "Measure" );

					TBuildingDataPtr pBuildingData = BuildingData::Create();
					pBuildingData->SetData( sTmplName, iMeasure );
					m_Data.Append( pBuildingData );
				}
			}
		}

		return true;
	}

	TBuildingDataPtr BuildingDataSet::BuildingData( SizeT iIndex )
	{
		TBuildingDataPtr pEmpty;

		if ( iIndex<0 )
		{
			return pEmpty;
		}

		if ( iIndex>=m_Data.Size() )
		{
			return pEmpty;
		}

		return m_Data[iIndex];
	}

	// - GameData implement -----------------------------------------------------------------
	__ImplementClass( Demo::GameData, 'DMGD', Core::RefCounted )
	__ImplementThreadSingleton( Demo::GameData )
	
	GameData::GameData()
		: m_BuildingData()
	{
		__ConstructThreadSingleton
	}

	GameData::~GameData()
	{
		__DestructThreadSingleton
	}

	bool GameData::LoadData( void )
	{
		bool bRet = false;
		bRet = m_BuildingData.LoadData();
		n_assert( bRet );

		return true;
	}

	TBuildingDataPtr GameData::BuildingData( SizeT iIndex )
	{
		return m_BuildingData.BuildingData( iIndex );
	}
}