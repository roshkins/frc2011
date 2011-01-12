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

#include "ArmJoint.hpp"

ArmJoint::ArmJoint(int motor_port, int encoder_port1, int encoder_port2)
{
	mMotor = new Jaguar(motor_port);
	mEncoder = new Encoder(encoder_port1, encoder_port2, true, Encoder::k4X);
}

ArmJoint::~ArmJoint()
{
	delete mMotor;
	delete mEncoder;
}

void ArmJoint::ToAngle(int angle)
{
	// TODO: Refactor into individual module (usable by both ArmJoint and Clamp)
	int diff = abs(angle - mEncoder->Get() % 360);
	
	mMotor->Set(1.0f);
	Wait(0.0015);
	mMotor->Set(0.0f);
	
	int new_diff = abs(angle - mEncoder->Get() % 360);
	
	while (new_diff > diff)
	{
		mMotor->Set(1.0f);
		new_diff = abs(angle - mEncoder->Get() % 360);
		Wait(0.0005); // 5ms is the update time for a Jaguar
	}
	
	while (new_diff < diff)
	{
		mMotor->Set(1.0f);
		new_diff = abs(angle - mEncoder->Get() % 360);
		Wait(0.0005); // 5ms is the update time for a Jaguar
	}
}
