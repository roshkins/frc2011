/*
 * Copyright 2011 Colin Warren <colin@colinwyattwarren.com>. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY Colin Warren ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL Colin Warren OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * The views and conclusions contained in the software and documentation are those of the
 * authors and should not be interpreted as representing official policies, either expressed
 * or implied, of Colin Warren.
 */

#pragma once
#include "defs.h"

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
	
	// Represents the state of the clamp
	enum ClampState
	{
		OPEN,
		CLOSED
	};
	
	Task *mCurrentTask;     // Currently executing task
	Task *mClampTask;       // Currently executing clamp task
	ArmState mArmState;     // State of the arm
	ClampState mClampState; // State of the clamp
};

/* I thoroughly hate that I have to do this, but unfortunately
 * they force me to use C functions as the argument to the Task
 * constructor, so I have to resort to this horrible hackery.
 */ 
void __Arm_ToLowPegs();
void __Arm_ToMediumPegs();
void __Arm_ToHighPegs();
void __Arm_OpenClamp();
void __Arm_CloseClamp();

// Unfortunately, macros calling macros is the simplest way to do this.
#define ARM_CHANGE_TASK(__FUNC) CHANGE_TASK("arm", mCurrentTask, __FUNC)
#define ARM_CLAMP_CHANGE_TASK(__FUNC) CHANGE_TASK("clamp", mClampTask, __FUNC)

