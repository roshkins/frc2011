/*
 * Copyright 2011 Marin Robotics (see AUTHORS file). All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, are
 * permitted provided that the following conditions are met:
 * 
 *   1. Redistributions of source code must retain the above copyright notice, this list of
 *      conditions and the following disclaimer.
 * 
 *   2. Redistributions in binary form must reproduce the above copyright notice, this list
 *      of conditions and the following disclaimer in the documentation and/or other materials
 *      provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY Marin Robotics ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL Marin Robotics OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * The views and conclusions contained in the software and documentation are those of the
 * authors and should not be interpreted as representing official policies, either expressed
 * or implied, of Marin Robotics.
 */

#pragma once
#include "defs.h"
#include "ArmJoint.hpp"
#include "Clamp.hpp"
#include <cstdarg>

class Arm
{
public:
	Arm();
	~Arm();
	
	// Moves the arm to the respective row of pegs
	void ToLowPegs();    // 3ft
	void ToMediumPegs(); // 6ft
	void ToHighPegs();   // 9ft
	
	// Opens and closes the clamp
	void OpenClamp();
	void CloseClamp();
	
	// Data getters
	bool IsRetracted();
	bool IsClampOpen();
	bool AtLowPegs();    // 3ft
	bool AtMediumPegs(); // 6ft
	bool AtHighPegs();   // 9ft
	
private:
	// Represents the state of the arm
	enum ArmState
	{
		RETRACTED,
		LOW_PEGS,
		MEDIUM_PEGS,
		HIGH_PEGS
	};
	
	/* I really have to commend FIRST for their dedication
	 * to a completely insane way to pass arguments. Maybe
	 * they should have us create Jaguars by calling the
	 * CreateVictorInstanceByReferenceImplementation method
	 * in the ElectricServo class.
	 * 
	 * Why can't they just let me pass a void** in?
	 */
	struct DataRepHackThing
	{
		ArmJoint *aj1;
		ArmJoint *aj2;
		Clamp *cl;
	};
	
	Task *mCurrentTask;     // Currently executing task
	Task *mClampTask;       // Currently executing clamp task
	ArmState mArmState;     // State of the arm
	ArmJoint *mArmJoint1;   // Joint #1
	ArmJoint *mArmJoint2;   // Joint #2
	Clamp *mClamp;          // Clamp
	DataRepHackThing mArgs; // Horrible thing
};

/* I thoroughly hate that I have to do this, but unfortunately
 * they force me to use C functions as the argument to the Task
 * constructor, so I have to resort to this horrible hackery.
 */ 
void __Arm_ToLowPegs();
void __Arm_ToMediumPegs();
void __Arm_ToHighPegs();
void __Arm_OpenClamp(...);
void __Arm_CloseClamp(...);
void __Arm_OpenClampImpl(Clamp *clamp);
void __Arm_CloseClampImpl(Clamp *clamp);

// Unfortunately, macros calling macros is the simplest way to do this.
#define ARM_CHANGE_TASK(__FUNC, ...) CHANGE_TASK("arm", mCurrentTask, __FUNC, __VA_ARGS__)
#define ARM_CLAMP_CHANGE_TASK(__FUNC, ...) CHANGE_TASK("clamp", mClampTask, __FUNC, __VA_ARGS__)

