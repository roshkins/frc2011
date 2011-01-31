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

// **************************************
// Headers
// **************************************
#include <WPILib.h>
#include <taskLib.h>
#include <DriverStationLCD.h>
#include <time.h>
#include <string>
#include "Logger.hpp"

// **************************************
// Input
// **************************************
#define JOYSTICK_1 1
#define JOYSTICK_2 2

#define ARM_ENCODER_1_1 1
#define ARM_ENCODER_1_2 2
#define ARM_ENCODER_2_1 3
#define ARM_ENCODER_2_2 4

#define LINE_SENSOR_1 5
#define LINE_SENSOR_2 6
#define LINE_SENSOR_3 7

#define CLAMP_SWITCH 8

// **************************************
// Output
// **************************************
#define DRIVE_MOTOR_1 1
#define DRIVE_MOTOR_2 2
#define DRIVE_MOTOR_3 3
#define DRIVE_MOTOR_4 4

#define ARM_MOTOR_1 5
#define ARM_MOTOR_2 6

#define CLAMP_MOTOR_1 7
#define CLAMP_MOTOR_2 8

// **************************************
// Functions/Macros
// **************************************
#define CHANGE_TASK(__NAME, __TASK, __FUNC, ...) \
	if (__TASK) \
	{ \
		(__TASK)->Stop(); \
		delete (__TASK); \
	} \
	(__TASK) = new Task(__NAME, (FUNCPTR) __FUNC); \
	(__TASK)->Start(__VA_ARGS__);
