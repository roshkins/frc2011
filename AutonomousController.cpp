#include "AutonomousController.hpp"

AutonomousController::AutonomousController()
{
	mRobotDrive = new RobotDrive(DRIVE_MOTOR_1, DRIVE_MOTOR_2, DRIVE_MOTOR_3, DRIVE_MOTOR_4);
	mLineSensorLeft = new DigitalInput(LINE_SENSOR_1);
	mLineSensorCenter = new DigitalInput(LINE_SENSOR_2);
	mLineSensorRight = new DigitalInput(LINE_SENSOR_3);
	mArm = new Arm();
	mLogger = &Logger::GetInstance();
}

void AutonomousController::Run()
{
	LineFollow();
	PlaceGamePiece();
}

void AutonomousController::LineFollow()
{
	
}

void AutonomousController::PlaceGamePiece()
{
	
}
