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

#include "Arm.hpp"

struct DataRepHackThing
{
	ArmJoint *aj1;
	ArmJoint *aj2;
	Clamp *cl;
};

static DataRepHackThing *mArgs; // I **HATE** VxWorks; if only they could just give me an
                                // embedded Debian system and the root password.

Arm::Arm()
{
	mCurrentTask = NULL;
	mClampTask = NULL;
	mArmState = RETRACTED;
	mArmJoint1 = new ArmJoint(ARM_MOTOR_1, ARM_ENCODER_1_1, ARM_ENCODER_1_2);
	mArmJoint2 = new ArmJoint(ARM_MOTOR_2, ARM_ENCODER_2_1, ARM_ENCODER_2_2);
	if (!mArgs)
	{
		mArgs = new DataRepHackThing();
		mArgs->aj1 = mArmJoint1;
		mArgs->aj2 = mArmJoint2;
		mArgs->cl = mClamp;
	}
}

Arm::~Arm()
{
	if (mCurrentTask)
		delete mCurrentTask;
}

bool Arm::IsRetracted()
{
	return (mArmState == RETRACTED);
}

bool Arm::IsClampOpen()
{
	return mClamp->IsOpen();
}

bool Arm::AtLowPegs()
{
	return (mArmState == LOW_PEGS);
}

bool Arm::AtMediumPegs()
{
	return (mArmState == MEDIUM_PEGS);
}

bool Arm::AtHighPegs()
{
	return (mArmState == HIGH_PEGS);
}

// All of these are called async
void __Arm_ToLowPegs()
{
	// TODO: Implement Arm::ToLowPegs
}

void __Arm_ToMediumPegs()
{
	// TODO: Implement Arm::ToMediumPegs
}

void __Arm_ToHighPegs()
{
	// TODO: Implement Arm::ToHighPegs
}

void __Arm_OpenClamp(...)
{
	mArgs->cl->Open();
}

void __Arm_CloseClamp(...)
{
	mArgs->cl->Close();
}

void Arm::ToLowPegs()
{
	// TODO: Base off of sensor values, don't assume
	mArmState = LOW_PEGS;
	ARM_CHANGE_TASK(__Arm_ToLowPegs);
}

void Arm::ToMediumPegs()
{
	// TODO: Base off of sensor values, don't assume
	mArmState = MEDIUM_PEGS;
	ARM_CHANGE_TASK(__Arm_ToMediumPegs);
}

void Arm::ToHighPegs()
{
	// TODO: Base off of sensor values, don't assume
	mArmState = HIGH_PEGS;
	ARM_CHANGE_TASK(__Arm_ToMediumPegs);
}

void Arm::OpenClamp()
{
	ARM_CLAMP_CHANGE_TASK(__Arm_OpenClamp);
}

void Arm::CloseClamp()
{
	ARM_CLAMP_CHANGE_TASK(__Arm_CloseClamp);
}
