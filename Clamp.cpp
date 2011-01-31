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

#include "Clamp.hpp"

Clamp::Clamp()
{
	mMotor1 = new Jaguar(CLAMP_MOTOR_1);
	mMotor2 = new Jaguar(CLAMP_MOTOR_2);
	mSwitch = new AnalogChannel(CLAMP_SWITCH);
	mTask = new Task("clamp", (FUNCPTR) __Clamp_PID_Loop);
	
	mTask->Start((UINT32) this);
	mCurrentAction = IDLE;
}

Clamp::~Clamp()
{
	delete mMotor1;
	delete mMotor2;
	delete mSwitch;
	delete mTask;
}

Jaguar *Clamp::Motor1()
{
	return mMotor1;
}

Jaguar *Clamp::Motor2()
{
	return mMotor2;
}

Clamp::ClampAction &Clamp::CurrentAction()
{
	return mCurrentAction;
}

void __Clamp_PID_Loop(Clamp *clamp, ...)
{
	clamp_pid_loop_start: // let thy raptors come forth from the walls and eat me
	double cm_1 = 0.0, cm_2 = 0.0;
	long cat = 0, cast = 0; // long cat is long
	time_t ct;
	Clamp::ClampAction as_p = Clamp::IDLE, as_c;
	
	while (true)
	{
		as_c = clamp->CurrentAction();
		ct = time(NULL);
		
		if (as_c != as_p)
		{
			switch (as_c)
			{
			case Clamp::IDLE:
				cm_1 = 0.0;
				cm_2 = 0.0;
				cast = 0;
				break;
			case Clamp::GRAB:
				if (cast == 0)
					cast = ct;
				if (cat > 3000)
				{
					clamp->CurrentAction() = Clamp::IDLE;
					goto clamp_pid_loop_start;
				}
				cat = (ct - cast);
				cm_1 = 1.0;
				cm_2 = 1.0;
				break;
			case Clamp::SPIT:
				if (cast == 0)
					cast = ct;
				if (cat > 3000)
				{
					clamp->CurrentAction() = Clamp::IDLE;
					goto clamp_pid_loop_start;
				}
				cat = (ct - cast);
				cm_1 = -1.0;
				cm_2 = -1.0;
				break;
			case Clamp::ROTATE_UP:
				if (cast == 0)
					cast = ct;
				if (cat > 3000)
				{
					clamp->CurrentAction() = Clamp::IDLE;
					goto clamp_pid_loop_start;
				}
				cat = (ct - cast);
				cm_1 = 1.0;
				cm_2 = -1.0;
				break;
			case Clamp::ROTATE_DOWN:
				if (cast == 0)
					cast = ct;
				if (cat > 3000)
				{
					clamp->CurrentAction() = Clamp::IDLE;
					goto clamp_pid_loop_start;
				}
				cat = (ct - cast);
				cm_1 = -1.0;
				cm_2 = 1.0;
				break;
			default:
				clamp->CurrentAction() = Clamp::IDLE;
				continue;
			}
		}
		
		clamp->Motor1()->Set(cm_1);
		clamp->Motor2()->Set(cm_2);
		
		as_p = as_c;
	}
}

void Clamp::Grab()
{
	mCurrentAction = GRAB;
}

void Clamp::Spit()
{
	mCurrentAction = SPIT;
}

void Clamp::RotateUp()
{
	mCurrentAction = ROTATE_UP;
}

void Clamp::RotateDown()
{
	mCurrentAction = ROTATE_DOWN;
}

bool Clamp::HasPiece()
{
	return mSwitch->GetVoltage() > 1.0f;
}
