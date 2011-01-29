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
#include "Clamp.hpp"
#include <list>

class Arm
{
public:
	enum ArmState
	{
		RETRACTED,
		SLOT,
		GROUND,
		LOW_PEGS,
		MEDIUM_PEGS,
		HIGH_PEGS
	};
	
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
	ArmState Status();
	Jaguar *JointMotor1();
	Jaguar *JointMotor2();
	Encoder *JointEncoder1();
	Encoder *JointEncoder2();
	
private:
	Task *mCurrentTask;     // Currently executing task
	Task *mClampTask;       // Currently executing clamp task
	ArmState mArmState;     // State of the arm
	Jaguar *mJointMotor1;   // Joint #1
	Jaguar *mJointMotor2;   // Joint #2
	Encoder *mJointEncoder1;//
	Encoder *mJointEncoder2;//
	Clamp *mClamp;          // Clamp
};

void __Arm_PID_Loop(Arm *arm);

