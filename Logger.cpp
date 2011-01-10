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

#include "Logger.hpp"

Logger &Logger::GetInstance()
{
	static Logger logger;
	return logger;
}

Logger::Logger()
{
	mOut = DriverStationLCD::GetInstance();
}

void Logger::info(std::string data)
{
	print("NFO: " + data);
}

void Logger::debug(std::string data)
{
	print("DBG: " + data);
}

void Logger::warning(std::string data)
{
	print("WRN: " + data);
}

void Logger::error(std::string data)
{
	print("ERR: " + data);
}

void Logger::print(std::string data)
{
	// If an empty line at the end is empty, use it
	for (int i = 5; i >= 0; i--)
	{
		if (mLines[i] == "")
		{
			mLines[i] = data;
			return;
		}
	}
	
	// Else, bump the lines up one and put it at the end
	for (int i = 1; i < 6; i++)
		mLines[i - 1] = mLines[i];
	mLines[5] = data;
	
	// Draw the lines to the screen
	draw();
}

void Logger::draw()
{
	mOut->PrintfLine(DriverStationLCD::kUser_Line1, mLines[0].c_str());
	mOut->PrintfLine(DriverStationLCD::kUser_Line2, mLines[1].c_str());
	mOut->PrintfLine(DriverStationLCD::kUser_Line3, mLines[2].c_str());
	mOut->PrintfLine(DriverStationLCD::kUser_Line4, mLines[3].c_str());
	mOut->PrintfLine(DriverStationLCD::kUser_Line5, mLines[4].c_str());
	mOut->PrintfLine(DriverStationLCD::kUser_Line6, mLines[5].c_str());
}
