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

#include "MainRobot.hpp"

MainRobot::MainRobot(void)
{
	mDrive = new Drive();
	mWatchdog = &GetWatchdog();
	mArm = new Arm();
	mLogger = &Logger::GetInstance();
	
	mWatchdog->SetExpiration(0.1);
	mDrive->setDriveMode(Drive::TANK);
}

MainRobot::~MainRobot()
{
	delete mDrive;
	// Don't delete watchdog (we didn't allocate it)
	delete mArm;
	// Don't delete logger (shared instance)
}

void MainRobot::Autonomous(void)
{
	// @author Colin "Danger" Warren
	mWatchdog->SetEnabled(false);
	
	// Follow line (automate this)
	
	// Raise Arm to highest pegs
	mArm->ToHighPegs();
}

void MainRobot::OperatorControl(void)
{
	// Enables the watchdog (if comms are dropped, commit suicide)
	mWatchdog->SetEnabled(true);
	
	time_t startTime;
	
	while (IsOperatorControl())
	{
		// Grab start time
		startTime = time(NULL);
		
		mWatchdog->Feed();
		mDrive->Update();
		
		// Main loop time should be 50ms, if greater, log a warning
		if (time(NULL) > startTime + 0.005)
			mLogger->warning("Main loop took more than 50ms!");
		else
			Wait((startTime + 0.005) - time(NULL));
	}
}
