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
#include <cmath>

Arm::Arm()
{
	mCurrentTask = new Task("arm", (FUNCPTR) __Arm_PID_Loop);
	mArmState = RETRACTED;
	mJointMotor1 = new Jaguar(ARM_MOTOR_1);
	mJointEncoder1 = new Encoder(ARM_ENCODER_1_1, ARM_ENCODER_1_2);
	mJointMotor2 = new Jaguar(ARM_MOTOR_2);
	mJointEncoder2 = new Encoder(ARM_ENCODER_2_1, ARM_ENCODER_2_2);
	
	mCurrentTask->Start((UINT32) this);
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

bool Arm::ClampHasPiece()
{
	return mClamp->HasPiece();
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

Arm::ArmState Arm::Status()
{
	return mArmState;
}

Jaguar *Arm::JointMotor1()
{
	return mJointMotor1;
}

Jaguar *Arm::JointMotor2()
{
	return mJointMotor2;
}

Encoder *Arm::JointEncoder1()
{
	return mJointEncoder1;
}

Encoder *Arm::JointEncoder2()
{
	return mJointEncoder2;
}


void __Arm_PID_Loop(Arm *arm, ...)
{
	const double aj1_m = 360 * 1000, aj2_m = 360 * 2.5;
	double aj1_v, aj2_v, aj1_vo, aj2_vo, aj1_t = 0, aj2_t = 0, aj1_p, aj2_p;
	Arm::ArmState as_p = Arm::RETRACTED, as_c;
	Timer timer;
	
	arm->JointEncoder1()->Reset();
	arm->JointEncoder2()->Reset();
	timer.Start();
	
	while (true)
	{
		timer.Reset();
		aj1_v = arm->JointEncoder1()->Get();
		aj2_v = arm->JointEncoder2()->Get();
		as_c = arm->Status();
		
		if (as_c != as_p)
		{
			// TODO: Calibrate settings
			switch (as_c)
			{
			case Arm::RETRACTED:
				aj1_t = 0;
				aj2_t = 0;
				break;
			case Arm::SLOT:
				aj1_t = 0;
				aj2_t = 0;
				break;
			case Arm::GROUND:
				aj1_t = 0;
				aj2_t = 0;
				break;
			case Arm::LOW_PEGS:
				aj1_t = 0;
				aj2_t = 0;
				break;
			case Arm::MEDIUM_PEGS:
				aj1_t = 0;
				aj2_t = 0;
				break;
			case Arm::HIGH_PEGS:
				aj1_t = 0;
				aj2_t = 0;
				break;
			default:
				// Retract it if status unknown
				aj1_t = 0;
				aj2_t = 0;
				break;
			}
		}
		
		aj1_p = log(1 + (((aj1_v - aj1_t) * 9) / aj1_m));
		aj2_p = log(1 + (((aj2_v - aj2_t) * 9) / aj2_m));
		
		// TODO: Calibrate this thing so it doesn't wiggle too much.
		if (aj1_p < 0.1)
			aj1_p = 0;
		if (aj2_p < 0.1)
			aj2_p = 0;
		
		arm->JointMotor1()->Set(aj1_p);
		arm->JointMotor2()->Set(aj2_p);
		
		as_p = as_c;
		aj1_vo = aj1_v;
		aj2_vo = aj2_v;
		
		if (timer.Get() < 0.0050)
			Wait(0.0050 - timer.Get());
	}
}

void Arm::ToLowPegs()
{
	mArmState = LOW_PEGS;
}

void Arm::ToMediumPegs()
{
	mArmState = MEDIUM_PEGS;
}

void Arm::ToHighPegs()
{
	mArmState = HIGH_PEGS;
}

void Arm::ClampGrab()
{
	mClamp->Grab();
}

void Arm::ClampSpit()
{
	mClamp->Spit();
}

void Arm::ClampRotateUp()
{
	mClamp->RotateUp();
}

void Arm::ClampRotateDown()
{
	mClamp->RotateDown();
}
