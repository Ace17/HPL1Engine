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
#pragma once

#include "system/String.h"

namespace hpl
{
	class cSoundManager;

	class iSoundEnvironment
	{
	public:
		iSoundEnvironment ( ){}
		virtual ~iSoundEnvironment () {}

		virtual bool CreateFromFile(const tString &asFile){ return false;}

		tString& GetName() { return mstrName; }
		tString& GetFileName() { return msFileName; }
		void SetFileName(const tString & asFileName){ msFileName = asFileName; }

	protected:
		tString mstrName;
		tString msFileName;
	};

}



