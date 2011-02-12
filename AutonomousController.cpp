#include "AutonomousController.hpp"
#include "WPILib.h"

const bool isLeft = true;

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

void TurnLeftUntilAligned()
{
	//This implements a loop that turns the same direction (left) until the center line follower is activated, and the two on the sides are not.
}

void TurnRightUntilAligned()
{
	//This implements a loop that turns the same direction (right) until the center line follower is activated, and the two on the sides are not.
}

void AutonomousController::LineFollow()
{
	RobotDrive *drive;			// robot drive base object
	DigitalInput *left;			// digital inputs for line tracking sensors
	DigitalInput *middle;
	DigitalInput *right;
	DriverStation *ds;			// driver station object for getting selections
	
	drive = new RobotDrive(1, 2);
	drive->SetInvertedMotor(RobotDrive::kFrontRightMotor, true);
	drive->SetInvertedMotor(RobotDrive::kFrontLeftMotor, true);
	drive->SetInvertedMotor(RobotDrive::kRearRightMotor, true);
	drive->SetInvertedMotor(RobotDrive::kRearLeftMotor, true);
	drive->SetExpiration(15);
	left = new DigitalInput(1);
	middle = new DigitalInput(2);
	right = new DigitalInput(3);
	ds = DriverStation::GetInstance();	// driver station instance for digital I/O
	double defaultSteeringGain = 0.65;	// default value for steering gain

	int binaryValue;
	int previousValue = 0;
	double steeringGain;

	// The power profiles for the straight and forked robot path. They are
	// different to let the robot drive more slowly as the robot approaches
	// the fork on the forked line case.
	double forkProfile[] = {0.70, 0.70, 0.55, 0.60, 0.60, 0.50, 0.40, 0.0};
	double straightProfile[] = {0.70, 0.70, 0.60, 0.60, 0.35, 0.35, 0.35, 0.0};

	double *powerProfile;  // the selected power profile
	
	// Set the straight vs. forked path variables as read from the digital inputs or the I/O Setup panel on the driver station.
	bool straightLine = ds->GetDigitalIn(1);
	powerProfile = (straightLine) ? straightProfile : forkProfile;
	double stopTime = (straightLine) ? 2.0 : 4.0;
	bool goLeft = !ds->GetDigitalIn(2) && !straightLine;
	printf("StraightLine: %d\n", straightLine);
	printf("GoingLeft: %d\n", goLeft);

	bool atCross = false;	// true when robot has reached end

	// Set up timer and use the timer to pick values from the power profile arrays.
	Timer *timer = new Timer();
	timer->Start();
	timer->Reset();
	
	int oldTimeInSeconds = -1;
	double time;
	double speed, turn;

	// Loop until we hit the "T" at the end.
	while (!atCross) {
		time = timer->Get();
		int timeInSeconds = (int) time;
		int leftValue = left->Get() ? 1 : 0;	// read the line tracking sensors
		int middleValue = middle->Get() ? 1 : 0;
		int rightValue = right->Get() ? 1 : 0;

	    // Compute the single value from the 3 sensors. Notice that the bits for the outside sensors are flipped depending on left or right fork. Also the sign of the steering direction is different for left/right.
		if (goLeft) {
			binaryValue = leftValue * 4 + middleValue * 2 + rightValue;
			steeringGain = -defaultSteeringGain;
		} else {
			binaryValue = rightValue * 4 + middleValue * 2 + leftValue;
			steeringGain = defaultSteeringGain;
		}

		speed = powerProfile[timeInSeconds];	// Speed value
		turn = 0;								// Default is no turn

		switch (binaryValue) {
			case 1:					// Just the inside sensor - drive straight
				turn = 0;
				break;
			case 7:					// All sensors - maybe at the "T"
			if (time> stopTime) {
				atCross = true;
				speed = 0;
			}
			break;
			case 0:					// No sensors - apply previous correction
			if (previousValue == 0 || previousValue == 1) {
				turn = steeringGain;
			}
			else {
				turn = -steeringGain;
			}
			//comment here 
			break;
			default:				// Anything else, steer back to the line
			turn = -steeringGain;
		}
		// Useful debugging output for tuning power profile and steering gain
		if(binaryValue != previousValue)
			printf("Time: %2.2f sensor: %d speed: %1.2f turn: %1.2f atCross: %d\n", time, binaryValue, speed, turn, atCross);
		// Move the robot forward
		drive->ArcadeDrive(speed, turn);
		if (binaryValue != 0) previousValue = binaryValue;
		oldTimeInSeconds = timeInSeconds;
		Wait(0.01);
	}
	// Stop driving when finished
	drive->ArcadeDrive(0.0, 0.0);
}


void AutonomousController::PlaceGamePiece()
{

}

