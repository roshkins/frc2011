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

#include "Drive.hpp"

Drive::Drive()
{
	mRobotDrive = new RobotDrive(DRIVE_MOTOR_1, DRIVE_MOTOR_2);
	mJoystick1 = new Joystick(JOYSTICK_1);
	mJoystick2 = new Joystick(JOYSTICK_2);
	
	mRobotDrive->SetExpiration(0.1);
	mRobotDrive->SetSafetyEnabled(true);
}

Drive::~Drive()
{
	delete mRobotDrive;
	delete mJoystick1;
	delete mJoystick2;
}

void Drive::setDriveMode(DriveMode mode)
{
	mMode = mode;
}

void Drive::Update()
{
	switch (mMode)
	{
	case ARCADE:
		mRobotDrive->ArcadeDrive(mJoystick1);
		break;
	case TANK:
		mRobotDrive->TankDrive(mJoystick1, mJoystick2);
		break;
	}
}
