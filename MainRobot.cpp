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

#include "MainRobot.hpp"

MainRobot::MainRobot(void)
{
	mRobotDrive = new RobotDrive(DRIVE_MOTOR_1, DRIVE_MOTOR_2);
	mJoystick = new Joystick(JOYSTICK_1);
	mWatchdog = &GetWatchdog();
	mArm = new Arm();
	mLogger = &Logger::GetInstance();
	
	mWatchdog->SetExpiration(0.1);
	mRobotDrive->SetExpiration(0.1);
}

MainRobot::~MainRobot()
{
	delete mRobotDrive;
	delete mJoystick;
	// Don't delete watchdog (we didn't allocate it)
	delete mArm;
	// Don't delete logger (shared instance)
}

void MainRobot::Autonomous(void)
{
	// @author Colin "Danger" Warren
	mWatchdog->SetEnabled(false);
	mRobotDrive->SetSafetyEnabled(false);
	
	// Follow line (automate this)
	
	// Raise Arm to highest pegs
	mArm->ToHighPegs();
}

void MainRobot::OperatorControl(void)
{
	// Enables the watchdog (if comms are dropped, commit suicide)
	mWatchdog->SetEnabled(true);
	mRobotDrive->SetSafetyEnabled(true); // What does this do?
	
	time_t startTime;
	
	while (IsOperatorControl())
	{
		// Grab start time
		startTime = time(NULL);
		
		mWatchdog->Feed();
		mRobotDrive->ArcadeDrive(mJoystick);
		
		// Main loop time should be 50ms, if greater, log a warning
		if (time(NULL) > startTime + 0.005)
			mLogger->warning("Main loop took more than 50ms!");
		else
			Wait((startTime + 0.005) - time(NULL));
	}
}

#if 0
START_ROBOT_CLASS(MainRobot);
#endif
