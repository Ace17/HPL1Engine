/*
 * Copyright (C) 2006-2010 - Frictional Games
 *
 * This file is part of HPL1 Engine.
 *
 * HPL1 Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * HPL1 Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with HPL1 Engine.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "game/Updater.h"

#include "game/Updateable.h"
#include "system/LowLevelSystem.h"

namespace hpl {

	//////////////////////////////////////////////////////////////////////////
	// CONSTRUCTORS
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------

	cUpdater::cUpdater(iLowLevelSystem *apLowLevelSystem)
	{
		mpCurrentUpdates = NULL;

		mpLowLevelSystem = apLowLevelSystem;
	}

	//-----------------------------------------------------------------------

	//////////////////////////////////////////////////////////////////////////
	// PUBLIC METHOD
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------

	void cUpdater::OnDraw()
	{
		for(auto pUpdate : mlstGlobalUpdateableList)
		{
			pUpdate->OnDraw();
		}

		if(mpCurrentUpdates)
		{
			for(auto pUpdate : *mpCurrentUpdates)
			{
				pUpdate->OnDraw();
			}
		}
	}
	//-----------------------------------------------------------------------

	void cUpdater::OnPostSceneDraw()
	{
		for(auto pUpdate : mlstGlobalUpdateableList)
		{
			pUpdate->OnPostSceneDraw();
		}

		if(mpCurrentUpdates)
		{
			for(auto pUpdate : *mpCurrentUpdates)
			{
				pUpdate->OnPostSceneDraw();
			}
		}
	}

	void cUpdater::OnPostGUIDraw()
	{
		for(auto pUpdate : mlstGlobalUpdateableList)
		{
			pUpdate->OnPostGUIDraw();
		}

		if(mpCurrentUpdates)
		{
			for(auto pUpdate : *mpCurrentUpdates)
			{
				pUpdate->OnPostGUIDraw();
			}
		}
	}

	//-----------------------------------------------------------------------

	void cUpdater::OnPostBufferSwap()
	{
		for(auto pUpdate : mlstGlobalUpdateableList)
		{
			pUpdate->OnPostBufferSwap();
		}

		if(mpCurrentUpdates)
		{
			for(auto pUpdate : *mpCurrentUpdates)
			{
				pUpdate->OnPostBufferSwap();
			}
		}
	}

	//-----------------------------------------------------------------------

	void cUpdater::OnStart()
	{
		for(auto pUpdate : mlstGlobalUpdateableList)
		{
			pUpdate->OnStart();
		}

		for(auto& ContIt : m_mapUpdateContainer)
		{
			for(auto pUpdate : ContIt.second)
			{
				pUpdate->OnStart();
			}
		}
	}

	//-----------------------------------------------------------------------

	void cUpdater::Reset()
	{
		for(auto pUpdate : mlstGlobalUpdateableList)
		{
			pUpdate->Reset();
		}

		for(auto& ContIt : m_mapUpdateContainer)
		{
			for(auto pUpdate : ContIt.second)
			{
				pUpdate->Reset();
			}
		}
	}

	//-----------------------------------------------------------------------

	void cUpdater::OnExit()
	{
		for(auto pUpdate : mlstGlobalUpdateableList)
		{
			//Log(" Exiting %s\n",(*it)->GetName().c_str());
			pUpdate->OnExit();
		}

		for(auto& ContIt : m_mapUpdateContainer)
		{
			for(auto pUpdate : ContIt.second)
			{
				//Log(" Exiting %s\n",(*UpIt)->GetName().c_str());
				pUpdate->OnExit();
			}
		}
	}

	//-----------------------------------------------------------------------

	void cUpdater::Update(float afTimeStep)
	{
		for(auto pUpdate : mlstGlobalUpdateableList)
		{
			START_TIMING_EX(pUpdate->GetName().c_str(),game)
			pUpdate->Update(afTimeStep);
			STOP_TIMING(game)
		}

		if(mpCurrentUpdates)
		{
			for(auto pUpdate : *mpCurrentUpdates)
			{
				START_TIMING_EX(pUpdate->GetName().c_str(),game)
				pUpdate->Update(afTimeStep);
				STOP_TIMING(game)
			}
		}
	}

	//-----------------------------------------------------------------------


	bool cUpdater::SetContainer(tString asContainer)
	{
		tUpdateContainerMapIt it = m_mapUpdateContainer.find(asContainer);
		if(it == m_mapUpdateContainer.end()) return false;

		msCurrentUpdates = asContainer;
		if(msCurrentUpdates == "Default"){
			SetUpdateLogActive(true);
		}
		else {
			SetUpdateLogActive(false);
		}

		mpCurrentUpdates = &it->second;

		return true;
	}

	tString cUpdater::GetCurrentContainerName()
	{
		if(mpCurrentUpdates==NULL) return "";

		return msCurrentUpdates;

	}

	//-----------------------------------------------------------------------

	bool cUpdater::AddContainer(tString asName)
	{
		//Create the value for the map with key and Updateable
		tUpdateContainerMap::value_type val = tUpdateContainerMap::value_type(
														asName, tUpdateableList());
		//Add it to the map
		m_mapUpdateContainer.insert(val);

		return true;
	}

	//-----------------------------------------------------------------------

	bool cUpdater::AddUpdate(tString asContainer, iUpdateable* apUpdate)
	{
		if(apUpdate==NULL){
			Error("Couldn't add NULL updatable!");
			return false;
		}

		//Search the map for the container name
		tUpdateContainerMapIt it = m_mapUpdateContainer.find(asContainer);
		if(it == m_mapUpdateContainer.end()) return false;

		//Add the updatable
		it->second.push_back(apUpdate);

		return true;
	}

	//-----------------------------------------------------------------------

	bool cUpdater::AddGlobalUpdate(iUpdateable* apUpdate)
	{
		mlstGlobalUpdateableList.push_back(apUpdate);
		return true;
	}

	//-----------------------------------------------------------------------

	//////////////////////////////////////////////////////////////////////////
	// PRIVATE METHODS
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------



	//-----------------------------------------------------------------------
}
