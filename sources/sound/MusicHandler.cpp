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
#include "sound/MusicHandler.h"
#include "resources/Resources.h"
#include "system/LowLevelSystem.h"
#include "system/String.h"
#include "math/Math.h"
#include "sound/LowLevelSound.h"
#include "sound/SoundChannel.h"
#include "sound/SoundData.h"
#include "resources/SoundManager.h"


namespace hpl {

	//////////////////////////////////////////////////////////////////////////
	// CONSTRUCTORS
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------

	cMusicHandler::cMusicHandler(iLowLevelSound* apLowLevelSound, cResources* apResources)
	{
		mpLowLevelSound = apLowLevelSound;
		mpResources = apResources;

		mpMainSong = NULL;
		mpLock  = NULL;
		mbIsPaused = false;
	}

	//-----------------------------------------------------------------------

	cMusicHandler::~cMusicHandler()
	{
		if(mpMainSong){
			hplDelete(mpMainSong->mpStream);
			hplDelete(mpMainSong);
		}

		for(auto pSong : mlstFadingSongs)
		{
			hplDelete(pSong->mpStream);
			hplDelete(pSong);
		}
	}

	//-----------------------------------------------------------------------

	//////////////////////////////////////////////////////////////////////////
	// PUBLIC METHODS
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------

	bool cMusicHandler::Play(const tString& asFileName,float afVolume, float afFadeStepSize, bool abLoop)
	{
		bool bSongIsPlaying = false;

		if(mpLock!=NULL){
			mpLock->msFileName = asFileName;
			mpLock->mfVolume = afVolume;
			mpLock->mbLoop = abLoop;
			return true;
		}

		if(mpMainSong != NULL)
			if(asFileName == mpMainSong->msFileName) bSongIsPlaying = true;

		if(!bSongIsPlaying)
		{
			//Put the previous song in the fading queue
			if(mpMainSong != NULL)
			{
				mpMainSong->mfVolumeAdd = afFadeStepSize;
				mlstFadingSongs.push_back(mpMainSong);
			}

			//If the song to be played is in the fading queue, stop it.
			tMusicEntryListIt it = mlstFadingSongs.begin();
			while(it != mlstFadingSongs.end())
			{
				cMusicEntry* pSong = *it;
				if(pSong->msFileName == asFileName)
				{
					pSong->mfVolume= 0;
					pSong->mpStream->Stop();
					hplDelete(pSong->mpStream);
					hplDelete(pSong);

					it = mlstFadingSongs.erase(it);
				} else {
					it++;
				}
			}


			//add it and set its properties
			mpMainSong = hplNew( cMusicEntry, () );

			if(LoadAndStart(asFileName, mpMainSong,0,abLoop)==false){
				hplDelete(mpMainSong);
				mpMainSong = NULL;
				return false;
			}
		}
		else
		{
			if(mpMainSong->mfMaxVolume == afVolume)return true;
		}

		//Set Properties
		mpMainSong->mfMaxVolume = afVolume;
		mpMainSong->mbLoop = abLoop;

		if(mpMainSong->mfMaxVolume > mpMainSong->mfVolume)
			mpMainSong->mfVolumeAdd = afFadeStepSize;
		else
			mpMainSong->mfVolumeAdd = -afFadeStepSize;


		return true;
	}

	//-----------------------------------------------------------------------

	void cMusicHandler::Stop(float afFadeStepSize)
	{
		if(mpMainSong==NULL)return;

		if(afFadeStepSize<0)afFadeStepSize=-afFadeStepSize;

		mpMainSong->mfVolumeAdd = afFadeStepSize;
		if(afFadeStepSize==0){
			mpMainSong->mpStream->SetVolume(0);
			mpMainSong->mpStream->Stop();
			mpMainSong->mfVolume =0;
		}

		mlstFadingSongs.push_back(mpMainSong);
		mpMainSong = NULL;
	}

	//-----------------------------------------------------------------------

	void cMusicHandler::Pause()
	{
		if(mpMainSong != NULL)mpMainSong->mpStream->SetPaused(true);

		for(auto pSong : mlstFadingSongs) {
			pSong->mpStream->SetPaused(true);
		}

		mbIsPaused = true;
	}

	//-----------------------------------------------------------------------

	void cMusicHandler::Resume()
	{
		if(mpMainSong != NULL)mpMainSong->mpStream->SetPaused(false);

		for(auto pSong : mlstFadingSongs) {
			pSong->mpStream->SetPaused(false);
		}

		mbIsPaused = false;
	}

	//-----------------------------------------------------------------------


	void cMusicHandler::Lock(cMusicLock* apLock)
	{
		mpLock = apLock;
	}

	//-----------------------------------------------------------------------

	void cMusicHandler::UnLock()
	{
		mpLock = NULL;
	}

	//-----------------------------------------------------------------------

	cMusicEntry* cMusicHandler::GetCurrentSong()
	{
		return mpMainSong;
	}

	//-----------------------------------------------------------------------

	void cMusicHandler::Update(float afTimeStep)
	{
		if(mbIsPaused)return;

		if(mpMainSong != NULL)
		{
			if(mpMainSong->mpStream->IsPlaying()==false)
			{
				hplDelete(mpMainSong->mpStream);
				hplDelete(mpMainSong);
				mpMainSong = NULL;
			}
			else
			{
				//Update the main song
				mpMainSong->mfVolume+=mpMainSong->mfVolumeAdd*afTimeStep;

				if(mpMainSong->mfVolumeAdd>0)
				{
					if(mpMainSong->mfVolume>=mpMainSong->mfMaxVolume)
						mpMainSong->mfVolume= mpMainSong->mfMaxVolume;
				}
				else
				{
					if(mpMainSong->mfVolume<=mpMainSong->mfMaxVolume)
						mpMainSong->mfVolume= mpMainSong->mfMaxVolume;
				}


				if(mpMainSong->mpStream->GetVolume()!=mpMainSong->mfVolume)
				{
					mpMainSong->mpStream->SetVolume(mpMainSong->mfVolume);
				}
			}
		}


		//Update the fading songs
		tMusicEntryListIt it = mlstFadingSongs.begin();
		while(it != mlstFadingSongs.end())
		{
			cMusicEntry* pSong = *it;
			pSong->mfVolume-=pSong->mfVolumeAdd*afTimeStep;

			if(pSong->mfVolume<=0)
			{
				pSong->mfVolume= 0;
				pSong->mpStream->Stop();
				hplDelete(pSong->mpStream);
				hplDelete(pSong);

				it = mlstFadingSongs.erase(it);
			}
			else
			{
				pSong->mpStream->SetVolume(pSong->mfVolume);
				it++;
			}
		}
	}

	//-----------------------------------------------------------------------

	//////////////////////////////////////////////////////////////////////////
	// PRIVATE METHODS
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------

	bool cMusicHandler::LoadAndStart(const tString& asFileName,cMusicEntry* apSong  ,float afVolume, bool abLoop)
	{
		iSoundData* pData = mpResources->GetSoundManager()->CreateSoundData(asFileName,true,abLoop);
		if(pData==NULL){
			Error("Couldn't load music '%s'\n",asFileName.c_str());
			return false;
		}

		iSoundChannel *pStream = pData->CreateChannel(256);
		if(pStream == NULL){
			Error("Couldn't stream music '%s'!\n",asFileName.c_str());
			return false;
		}

		apSong->msFileName = asFileName;
		apSong->mpStream = pStream;
		apSong->mpStream->SetVolume(afVolume);

		apSong->mpStream->Play();

		return true;
	}
	//-----------------------------------------------------------------------

}
