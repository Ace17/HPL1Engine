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

#include "math/MathTypes.h"
#include "math/PidController.h"
#include "math/Spring.h"

#include "game/SaveGame.h"

namespace hpl {

	//-------------------------------------------

	enum ePhysicsControllerType
	{
		ePhysicsControllerType_Pid,
		ePhysicsControllerType_Spring,
		ePhysicsControllerType_LastEnum
	};

	//-------------------------------------------

	enum ePhysicsControllerInput
	{
		ePhysicsControllerInput_JointAngle,
		ePhysicsControllerInput_JointDist,
		ePhysicsControllerInput_LinearSpeed,
		ePhysicsControllerInput_AngularSpeed,
		ePhysicsControllerInput_LastEnum
	};

	//-------------------------------------------

	enum ePhysicsControllerOutput
	{
		ePhysicsControllerOutput_Force,
		ePhysicsControllerOutput_Torque,
		ePhysicsControllerOutput_LastEnum
	};

	//-------------------------------------------

	enum ePhysicsControllerAxis
	{
		ePhysicsControllerAxis_X,
		ePhysicsControllerAxis_Y,
		ePhysicsControllerAxis_Z,
		ePhysicsControllerAxis_LastEnum
	};

	//-------------------------------------------

	enum ePhysicsControllerEnd
	{
		ePhysicsControllerEnd_Null,
		ePhysicsControllerEnd_OnDest,
		ePhysicsControllerEnd_OnMin,
		ePhysicsControllerEnd_OnMax,
		ePhysicsControllerEnd_LastEnum
	};

	//-------------------------------------------

	kSaveData_BaseClass(iPhysicsController)
	{
		kSaveData_ClassInit(iPhysicsController)
	public:
		tString msName;

		int mlBodyId;
		int mlJointId;

		float mfA;
		float mfB;
		float mfC;
		float mfDestValue;
		float mfMaxOutput;

		bool mbMulMassWithOutput;

		int mType;
		int mInputType;
		int mInputAxis;
		int mOutputType;
		int mOutputAxis;
		int mEndType;

		tString msNextController;

		bool mbActive;
		bool mbPaused;

		iSaveObject* CreateSaveObject(cSaveObjectHandler *apSaveObjectHandler,cGame *apGame){return NULL;}
		int GetSaveCreatePrio(){return 0;}
	};

	//-------------------------------------------



	class iPhysicsWorld;
	class iPhysicsJoint;
	class iPhysicsBody;

	class iPhysicsController : public iSaveObject
	{
	#ifdef __GNUC__
		typedef iSaveObject __super;
	#endif
	public:
		iPhysicsController(const tString &asName, iPhysicsWorld *apWorld);
		virtual ~iPhysicsController() = default;

		void Update(float afTimeStep);

		const tString& GetName(){ return msName;}

		void SetJoint(iPhysicsJoint *apJoint){ mpJoint = apJoint;}
		iPhysicsJoint* GetJoint(){ return mpJoint;}
		void SetBody(iPhysicsBody *apBody){ mpBody = apBody;}
		iPhysicsBody* GetBody(){ return mpBody;}

		bool IsActive(){ return mbActive;}
		void SetActive(bool abX);

		/*
		 * p in Pid and k in springs
		 */
		void SetA(float afA){ mfA = afA;}
		/*
		* i in Pid and b in springs
		*/
		void SetB(float afB){ mfB = afB;}
		/*
		* d in Pid and not used in springs
		*/
		void SetC(float afC){ mfC = afC;}

		void SetPidIntegralSize(int alSize);

		void SetType(ePhysicsControllerType aType){ mType = aType;}

		void SetDestValue(float afX){ mfDestValue = afX;}
		float GetDestValue(){ return mfDestValue;}

		void SetMaxOutput(float afX){ mfMaxOutput = afX;}

		void SetInputType(ePhysicsControllerInput aInput, ePhysicsControllerAxis aAxis){
			mInputType = aInput; mInputAxis = aAxis;}

		void SetOutputType(ePhysicsControllerOutput aOutput, ePhysicsControllerAxis aAxis){
			mOutputType = aOutput; mOutputAxis = aAxis;}
		void SetMulMassWithOutput(bool abX){ mbMulMassWithOutput = abX;}

		void SetEndType(ePhysicsControllerEnd aEnd){ mEndType = aEnd;}
		ePhysicsControllerEnd GetEndType(){ return mEndType;}

		void SetNextController(const tString &asName){ msNextController = asName;}
		const tString& GetNextController(){ return msNextController;}

		void SetLogInfo(bool abX){mbLogInfo = abX;}

		void SetPaused(bool abX){mbPaused = abX;}

		static bool mbUseInputMatrixFix;

		//SaveObject implementation
		virtual iSaveData* CreateSaveData();
		virtual void SaveToSaveData(iSaveData *apSaveData);
		virtual void LoadFromSaveData(iSaveData *apSaveData);
		virtual void SaveDataSetup(cSaveObjectHandler *apSaveObjectHandler, cGame *apGame);

	protected:
		cVector3f GetInputValue(ePhysicsControllerInput aInput);
		float GetOutputValue(float afError,float afInput, float afTimeStep);
		void AddOutputValue(ePhysicsControllerOutput aOutput, ePhysicsControllerAxis aAxis,
							float afVal);
		float GetAxisValue(ePhysicsControllerAxis aAxis, const cVector3f &avVec);

		iPhysicsWorld *mpWorld;
		tString msName;

		iPhysicsBody *mpBody;
		iPhysicsJoint *mpJoint;

		float mfA,mfB,mfC;

		float mfDestValue;
		float mfMaxOutput;

		bool mbMulMassWithOutput;

		ePhysicsControllerType mType;

		ePhysicsControllerInput mInputType;
		ePhysicsControllerAxis mInputAxis;

		ePhysicsControllerOutput mOutputType;
		ePhysicsControllerAxis mOutputAxis;

		ePhysicsControllerEnd mEndType;

		tString msNextController;

		cPidControllerf mPidController;

		bool mbActive;
		bool mbPaused;

		bool mbLogInfo;
	};
};
